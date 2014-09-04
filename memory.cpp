#include "memory.h"
#include "tileengine.h"
#include "mainwindow.h"

#define TEXT_SEGMENT 1
#define DATA_SEGMENT 2
#define HEAP_SEGMENT 3
#define STACK_SEGMENT 4
#define TILE_MAP 5
#define BG_TILE_SET 6
#define SP_TILE_SET 7
#define SPRITE_RAM 8
#define PALETTE 9
#define INPUT_BUTTON 10
#define OUT_OF_RANGE 0

#define NO_OF_SPRITES 64

#define OUT_OF_RANGE_EX_NO 4
#define NOT_HWORD_ALIGN_EX_NO 5
#define NOT_WORD_ALIGN_EX_NO 6
#define WRITE_IN_READ_ONLY_MEMORY_EX_NO 7

#define TILE_SIZE 16

bool isLittleEndian(){
    return MainWindow::isLittleEndian();
}

unsigned int getScreensHeightCount()
{
    return 2;
}

unsigned int getScreensWidthCount()
{
    return 2;
}

unsigned int Memory::getByteSegment(unsigned int addr) const
{
    if(addr >= textSegmentBaseAddress && addr < textSegmentBaseAddress + textSegmentPhysicalSize)
        return TEXT_SEGMENT;
    else if(addr >= dataSegmentBaseAddress && addr < dataSegmentBaseAddress + dataSegmentPhysicalSize)
        return DATA_SEGMENT;
    else if(addr >= heapSegmentBaseAddress && addr < heapSegmentBaseAddress + heapSegmentPhysicalSize)
        return HEAP_SEGMENT;
    //else if(addr >= stackSegmentLimitAddress - stackSegmentPhysicalSize && addr < stackSegmentLimitAddress)
    //    return STACK_SEGMENT;
    else if(addr >= tileMapBaseAddress && addr < tileMapBaseAddress + tileMapPhysicalSize)
        return TILE_MAP;
    else if(addr >= backgroundTileSetBaseAddress && addr < backgroundTileSetBaseAddress + backgroundTileSetPhysicalSize)
        return BG_TILE_SET;
    else if(addr >= spritesTileSetBaseAddress && addr < spritesTileSetBaseAddress + spritesTileSetPhysicalSize)
        return SP_TILE_SET;
    else if(addr >= spriteRamBaseAddress && addr < spriteRamBaseAddress + spriteRamPhysicalSize)
        return SPRITE_RAM;
    else if(addr >= paletteBaseAddress && addr < paletteBaseAddress + palettePhysicalSize)
        return PALETTE;
    else if(addr >= inputMemoryBaseAddress && addr < inputMemoryBaseAddress + inputMemoryPhysicalSize)
        return INPUT_BUTTON;
    else
        return OUT_OF_RANGE;
}


unsigned int Memory::getHWordSegment(unsigned int addr) const
{
    unsigned int firstByte = getByteSegment(addr);
    unsigned int lastByte = getByteSegment(addr+1);
    if(firstByte == OUT_OF_RANGE || lastByte == OUT_OF_RANGE)
        return OUT_OF_RANGE;
    return firstByte;
}
unsigned int Memory::getWordSegment(unsigned int addr) const
{
    unsigned int firstByte = getByteSegment(addr);
    unsigned int lastByte = getByteSegment(addr+3);
    if(firstByte == OUT_OF_RANGE || lastByte == OUT_OF_RANGE)
        return OUT_OF_RANGE;
    return firstByte;
}

Memory::Memory(QObject *parent): QObject(parent),
                    textSegmentBaseAddress (0x00000000),
                    dataSegmentBaseAddress (0x00010000),
                    heapSegmentBaseAddress (0x00020000),
                    //stackSegmentLimitAddress (0x80000000),
                    spriteRamBaseAddress (0x00123000),
                    paletteBaseAddress (0x00123200),
                    tileMapBaseAddress (0x00120000),
                    spritesTileSetBaseAddress (0x00110000),
                    backgroundTileSetBaseAddress (0x00100000),
                    textSegmentPhysicalSize (64 * 1024),
                    dataSegmentPhysicalSize (64 * 1024),
                    heapSegmentPhysicalSize (896 * 1024),
                    //stackSegmentPhysicalSize (128 * 1024),
                    spriteRamPhysicalSize (512),
                    palettePhysicalSize (1 * 1024),
                    inputMemoryPhysicalSize(4),
                    inputMemoryBaseAddress(0xffff0000),
                    screenWidth(512),
                    screenHeight(384)
{

    spritesTileSetPhysicalSize = 256 * TILE_SIZE * TILE_SIZE;
    backgroundTileSetPhysicalSize = 256 * TILE_SIZE * TILE_SIZE;

    tileMapPhysicalSize = screenHeight/TILE_SIZE * getScreensHeightCount() * screenWidth/TILE_SIZE * getScreensWidthCount();
    backgroundTileSet.resize(256);
    spritesTileSet.resize(256);
    tileMap.resize(screenHeight/TILE_SIZE * getScreensHeightCount());
    tileMap.fill( QVector<char>(screenWidth/TILE_SIZE * getScreensWidthCount() ));
    backgroundMatrix.resize(screenHeight/TILE_SIZE * getScreensHeightCount());
    backgroundMatrix.fill( QVector<sf::Sprite>(screenWidth/TILE_SIZE * getScreensWidthCount() ));
    spriteRam.resize(NO_OF_SPRITES);
    palette.resize(256);

    textSegment.resize(textSegmentPhysicalSize);
    dataSegment.resize(dataSegmentPhysicalSize);
    heapSegment.resize(heapSegmentPhysicalSize);
    inputMemory.resize(inputMemoryPhysicalSize / 2);
    //stackSegment.resize(stackSegmentPhysicalSize);
    textSegment.fill(0);
    dataSegment.fill(0);
    heapSegment.fill(0);
    //stackSegment.fill(0);

    for(int i=0; i<backgroundTileSet.size(); i++)
        backgroundTileSet[i].setPalette(&palette);
    for(int i=0; i<spritesTileSet.size(); i++)
        spritesTileSet[i].setPalette(&palette);

    for (int i = 0; i < backgroundMatrix.size(); i++)
        for (int j = 0; j < backgroundMatrix[i].size(); j++)
            backgroundMatrix[i][j].setPosition(j * TILE_SIZE, i * TILE_SIZE);

    for(int i=0; i<spriteRam.size(); i++)
        spriteRam[i].setTileSet(&spritesTileSet);

}

Memory::~Memory()
{

}

int Memory::getTextSegmentSize()
{
    return textSegment.size();
}

int Memory::getDataSegmentSize()
{
    return dataSegment.size();
}

int Memory::getHeapSegmentSize()
{
    return heapSegment.size();
}

int Memory::getStackSegmentSize()
{
    return heapSegment.size();
}


void Memory::storeByte(unsigned int addr, char data)
{
    int segment = getByteSegment(addr);
    if (segment == OUT_OF_RANGE){
        //emit raiseException(OUT_OF_RANGE_EX_NO);
        return;
    }else if(segment == TEXT_SEGMENT)
        textSegment[addr - textSegmentBaseAddress] = data;
    else if(segment == DATA_SEGMENT)
        dataSegment[addr - dataSegmentBaseAddress] = data;
    else if(segment == HEAP_SEGMENT)
        heapSegment[addr - heapSegmentBaseAddress] = data;
    //else if(segment == STACK_SEGMENT)
    //    stackSegment[addr - (stackSegmentLimitAddress - stackSegmentPhysicalSize)] = data;
    else if(segment == TILE_MAP){
        int w = screenWidth/TILE_SIZE * getScreensWidthCount();
        int r = (addr - tileMapBaseAddress)/w;
        int c = (addr - tileMapBaseAddress)%w;
        backgroundTileSet[tileMap[r][c]].removeSprite(&backgroundMatrix[r][c]);
        tileMap[r][c] = data;
        backgroundMatrix[r][c].setTexture(backgroundTileSet[(unsigned char)tileMap[r][c]].getTexture());
        backgroundTileSet[(unsigned char)tileMap[r][c]].addSprite(&backgroundMatrix[r][c]);
        emit renderNow();
    }else if(segment == BG_TILE_SET){
        backgroundTileSet[(addr >> 8)&0xff].storeByte(addr, data);
        emit renderNow();
    }else if(segment == SP_TILE_SET){
        spritesTileSet[(addr>>8)&0xff].storeByte(addr, data);
        emit renderNow();
    }else if(segment == SPRITE_RAM){
        spriteRam[(addr - spriteRamBaseAddress)>>3].storeByte(addr,data);
        emit renderNow();
    }else if(segment == PALETTE){
        palette[(addr - paletteBaseAddress)>>2].storeByte(addr&0x3,data);
        emit renderNow();
    }else if(segment == INPUT_BUTTON){
        //emit raiseException(WRITE_IN_READ_ONLY_MEMORY_EX_NO);
        return;
    }
}

char Memory::loadByte(unsigned int addr) const
{
    int segment = getByteSegment(addr);
    if (segment == OUT_OF_RANGE){
        //emit raiseException(OUT_OF_RANGE_EX_NO);
        return 0;
    }else if(segment == TEXT_SEGMENT)
        return textSegment[addr - textSegmentBaseAddress];
    else if(segment == DATA_SEGMENT)
        return dataSegment[addr - dataSegmentBaseAddress];
    else if(segment == HEAP_SEGMENT)
        return heapSegment[addr - heapSegmentBaseAddress];
    //else if(segment == STACK_SEGMENT)
    //    return stackSegment[addr - (stackSegmentLimitAddress - stackSegmentPhysicalSize)];
    else if(segment == TILE_MAP){
        int w = screenWidth/TILE_SIZE * getScreensWidthCount();
        int r = (addr - tileMapBaseAddress)/w;
        int c = (addr - tileMapBaseAddress)%w;
        return tileMap[r][c];
    }else if(segment == BG_TILE_SET)
        return backgroundTileSet[(addr >> 8)&0xff].loadByte(addr);
    else if(segment == SP_TILE_SET)
        return spritesTileSet[(addr>>8)&0xff].loadByte(addr);
    else if(segment == SPRITE_RAM)
        return spriteRam[(addr - spriteRamBaseAddress)>>3].loadByte(addr);
    else if(segment == PALETTE)
        return palette[(addr - paletteBaseAddress)>>2].loadByte(addr&0x3);
    else if(segment == INPUT_BUTTON){
        return ((addr % 2)?  (inputMemory[(addr - inputMemoryBaseAddress)/2]>>8)&0xff : inputMemory[(addr - inputMemoryBaseAddress)/2]&0xff);
    }else return 0;
}

unsigned char Memory::loadByteU(unsigned int addr) const
{
    return (unsigned char)loadByte(addr);
}

void Memory::storeHWord(unsigned int addr, short data)
{
    if(getHWordSegment(addr) == OUT_OF_RANGE){
        //emit raiseException(OUT_OF_RANGE_EX_NO);
        return;
    }if((addr & 1) != 0){
        //emit raiseException(NOT_HWORD_ALIGN_EX_NO);
        return;
    }
    if(isLittleEndian()) {
        storeByte(addr,data&0xff);
        storeByte(addr+1, data>>8);
    } else {
        storeByte(addr,data>>8);
        storeByte(addr+1, data&0xff);
    }
}

void Memory::storeWordL(unsigned int addr, short data){
        if(getHWordSegment(addr) == OUT_OF_RANGE){
            //emit raiseException(OUT_OF_RANGE_EX_NO);
            return;
        }

        if(isLittleEndian()) {
            storeByte(addr, (data&0xFF000000) >> 24);
            storeByte(addr+1, (data&0x00FF0000) >> 16);
        } else {
            storeByte(addr, (data&0x00FF0000) >> 16);
            storeByte(addr+1, (data&0xFF000000) >> 24);
        }
}

void Memory::storeWordR(unsigned int addr, short data){
    if(getHWordSegment(addr) == OUT_OF_RANGE){
        //emit raiseException(OUT_OF_RANGE_EX_NO);
        return;
    }

    if(isLittleEndian()) {
        storeByte(addr,data&0xff);
        storeByte(addr+1, data>>8);
    } else {
        storeByte(addr,data>>8);
        storeByte(addr+1, data&0xff);
    }
}


short Memory::loadHWord(unsigned int addr) const
{
    if(getHWordSegment(addr) == OUT_OF_RANGE){
        //emit raiseException(OUT_OF_RANGE_EX_NO);
        return 0;
    }if((addr & 1) != 0){
        //emit raiseException(NOT_HWORD_ALIGN_EX_NO);
        return 0;
    }
    if(isLittleEndian())
        return ((unsigned short)(loadByte(addr + 1) << 8) | loadByteU(addr));
    else
        return ((unsigned short)(loadByte(addr) << 8) | loadByteU(addr + 1));
}

short Memory::loadWordL(unsigned int addr) const{
        if(getHWordSegment(addr) == OUT_OF_RANGE){
            //emit raiseException(OUT_OF_RANGE_EX_NO);
            return 0;
        }

        if(isLittleEndian())
            return ((unsigned short)(loadByte(addr + 1) << 8) | loadByteU(addr));
        else
            return ((unsigned short)(loadByte(addr) << 8) | loadByteU(addr + 1));

}

short Memory::loadWordR(unsigned int addr) const{
    if(getHWordSegment(addr) == OUT_OF_RANGE){
        //emit raiseException(OUT_OF_RANGE_EX_NO);
        return 0;
    }

    if(isLittleEndian())
        return ((unsigned short)(loadByte(addr + 3) << 8) | loadByteU(addr + 2));
    else
        return ((unsigned short)(loadByte(addr + 2) << 8) | loadByteU(addr + 3));
}

unsigned short Memory::loadHWordU(unsigned int addr) const
{
    return (unsigned short)loadHWord(addr);
}

void Memory::storeWord(unsigned int addr, int data)
{
    if(getWordSegment(addr) == OUT_OF_RANGE){
        //emit raiseException(OUT_OF_RANGE_EX_NO);
        qDebug() << "Out of range.";
        return;
    }if((addr & 3) != 0){
        //emit raiseException(NOT_WORD_ALIGN_EX_NO);
        qDebug() << "Unallgined.";
        return;
    }
    if(isLittleEndian()) {
        storeByte(addr, data&0xff);
        storeByte(addr+1, (data>>8)&0xff);
        storeByte(addr+2, (data>>16)&0xff);
        storeByte(addr+3, (data>>24)&0xff);
    } else {
        storeByte(addr, (data>>24)&0xff);
        storeByte(addr+1, (data>>16)&0xff);
        storeByte(addr+2, (data>>8)&0xff);
        storeByte(addr+3, (data)&0xff);
    }
}

int Memory::loadWord(unsigned int addr) const
{
   // return 5;
    if(getWordSegment(addr) == OUT_OF_RANGE){
        //emit raiseException(OUT_OF_RANGE_EX_NO);
        qDebug() << "Out of range.";
        return 0;
    }if((addr & 3) != 0){
        //emit raiseException(NOT_WORD_ALIGN_EX_NO);
         qDebug() << "Unallgined.";
        return 0;
    }
    if(isLittleEndian()) {
        return (((unsigned int)(loadByte(addr + 3)) << 24) | ((unsigned int)(loadByteU(addr + 2)) << 16) | ((unsigned int)(loadByteU(addr + 1)) << 8) | (loadByteU(addr)));
    } else {
        return (((unsigned int)(loadByte(addr)) << 24) | ((unsigned int)(loadByteU(addr + 1)) << 16) | ((unsigned int)(loadByteU(addr + 2)) << 8) | (loadByteU(addr + 3)));
    }
}

unsigned int Memory::loadWordU(unsigned int addr)const
{
    return (unsigned int)loadWord(addr);
}
void Memory::storeConditional(unsigned int addr, int data)
{
    storeWord(addr, data);
}

int Memory::loadLinked(unsigned int addr) const
{
    return loadWord(addr);
}


void Memory::saveMemory(QString fileName, QVector<bool> segmentsToLoad)
{
    int count = 0;
    std::ofstream out;
    out.open(fileName.toStdString().c_str(), std::ofstream::binary);
    if(segmentsToLoad[0]){
        for(int i=0; i<textSegmentPhysicalSize; i++){
            out.put(loadByte(textSegmentBaseAddress + i));
            count++;
        }
    }
    if(segmentsToLoad[1]){
        for(int i=0; i<dataSegmentPhysicalSize; i++){
            out.put(loadByte(dataSegmentBaseAddress + i));
            count++;
        }
    }
    if(segmentsToLoad[2]){
        for(int i=0; i<backgroundTileSetPhysicalSize; i++){
            out.put(loadByte(backgroundTileSetBaseAddress + i));
            count++;
        }
    }
    if(segmentsToLoad[3]){
        for(int i=0; i<spritesTileSetPhysicalSize; i++){
            out.put(loadByte(spritesTileSetBaseAddress + i));
            count++;
        }
    }
    if(segmentsToLoad[4]){
        for(int i=0; i<tileMapPhysicalSize; i++){
            out.put(loadByte(tileMapBaseAddress + i));
            count++;
        }
    }
    if(segmentsToLoad[5]){
        for(int i=0; i<spriteRamPhysicalSize; i++){
            out.put(loadByte(spriteRamBaseAddress + i));
            count++;
        }
    }
    if(segmentsToLoad[6]){
        for(int i=0; i<palettePhysicalSize; i++){
            out.put(loadByte(paletteBaseAddress + i));
            count++;
        }
    }
    if(segmentsToLoad[7]){
        for(int i=0; i<heapSegmentPhysicalSize; i++){
            out.put(loadByte(heapSegmentBaseAddress + i));
            count++;
        }
    }


    qDebug() << "saved: "<<count<<" bytes\n";
    out.close();
}

int Memory::claculateLoadSize(const QVector<bool>& segments)
{
    int totalSize = 0;
    if(segments[0])
        totalSize += textSegmentPhysicalSize;
    if(segments[1])
        totalSize += dataSegmentPhysicalSize;
    if(segments[2])
        totalSize += backgroundTileSetPhysicalSize;
    if(segments[3])
        totalSize += spritesTileSetPhysicalSize;
    if(segments[4])
        totalSize += tileMapPhysicalSize;
    if(segments[5])
        totalSize += spriteRamPhysicalSize;
    if(segments[6])
        totalSize += palettePhysicalSize;
    if(segments[7])
        totalSize += heapSegmentPhysicalSize;
    return totalSize;
}

void Memory::loadMemory(QString fileName,  QVector<bool> segmentsToLoad)
{
    int count = 0;
    QFile in(fileName);
    in.open(QIODevice::ReadOnly);

    char* byte = new char;

    if(segmentsToLoad[0]){
        for(int i=0; i<textSegmentPhysicalSize; i++){
            in.getChar(byte);
            storeByte(textSegmentBaseAddress + i, *byte);
            if(count %1024 == 0)
                emit loadingNumberChanged(count / 1024);
            count++;
        }
    }
    if(segmentsToLoad[1]){
        for(int i=0; i<dataSegmentPhysicalSize; i++){
            in.getChar(byte);
            storeByte(dataSegmentBaseAddress + i, *byte);
            if(count %1024 == 0)
                emit loadingNumberChanged(count / 1024);
            count++;
        }
    }

    if(segmentsToLoad[2]){
        for(int i=0; i<backgroundTileSetPhysicalSize; i++){
            in.getChar(byte);
            storeByte(backgroundTileSetBaseAddress + i, *byte);
            if(count %1024 == 0)
                emit loadingNumberChanged(count / 1024);
            count++;
        }
    }

    if(segmentsToLoad[3]){
        for(int i=0; i<spritesTileSetPhysicalSize; i++){
            in.getChar(byte);
            storeByte(spritesTileSetBaseAddress + i, *byte);
            if(count %1024 == 0)
                emit loadingNumberChanged(count / 1024);
            count++;
        }
    }

    if(segmentsToLoad[4]){
        for(int i=0; i<tileMapPhysicalSize; i++){
            in.getChar(byte);
            storeByte(tileMapBaseAddress + i, *byte);
            if(count %1024 == 0)
                emit loadingNumberChanged(count / 1024);
            count++;
        }
    }

    if(segmentsToLoad[5]){
        for(int i=0; i<spriteRamPhysicalSize; i++){
            in.getChar(byte);
            storeByte(spriteRamBaseAddress + i, *byte);
            if(count %1024 == 0)
                emit loadingNumberChanged(count / 1024);
            count++;
        }
    }

    if(segmentsToLoad[6]){
        for(int i=0; i<palettePhysicalSize; i++){
            in.getChar(byte);
            storeByte(paletteBaseAddress + i, *byte);
            if(count %1024 == 0)
                emit loadingNumberChanged(count / 1024);
            count++;
        }
    }
    if(segmentsToLoad[7]){
        for(int i=0; i<heapSegmentPhysicalSize; i++){
            in.getChar(byte);
            storeByte(heapSegmentBaseAddress + i, *byte);
            if(count %1024 == 0)
                emit loadingNumberChanged(count / 1024);
            count++;
        }
    }


    qDebug() << "loaded: "<<count<<" bytes\n";
    in.close();
    emit loadingNumberChanged((claculateLoadSize(segmentsToLoad) + 1024 - 1)/1024);


}

void Memory::setTileEngine(TileEngine *engine)
{
    this->engine = engine;
}

TileEngine* Memory::getTileEngine() const
{
    return engine;
}

void Memory::updateKey(int keyCode, int controllerId, bool value)
{
    if(value){
        Uint16 mask = (Uint16(1) << keyCode);
        inputMemory[controllerId] = inputMemory[controllerId] | mask;
    } else{
        Uint16 mask = ~(Uint16(1) << keyCode);
        inputMemory[controllerId] = inputMemory[controllerId] & mask;
    }
}

/*
bool Memory::isValidWordL(int addr, int off) const
{
    if (addr - off > MemoryLimit)
    {
       // emit raiseException(OUT_OF_RANGE_EX_NO);
        return false;
    }
    return true;

}
bool Memory::isValidWordR(int addr, int off) const
{
    if (addr + off > MemoryLimit)
    {
        //emit raiseException(OUT_OF_RANGE_EX_NO);
        return false;
    }
    return true;

}

int Memory::loadWordL(int addr, int off) const
{
    if (!isValidWordL(addr, off))
        return 0;
    return (int) (memBytes[addr - off] << 8 |  memBytes[addr - off + 1]);
}

int Memory::loadWordR(int addr, int off) const
{
    if (!isValidWordR(addr, off))
        return 0;
    return (int) (memBytes[addr + off] << 8 |  memBytes[addr + off - 1] << 8);
}

void Memory::storeWordL(int addr, int off, int data)
{
    if (!isValidWordL(addr, off))
        return;
    memBytes[addr - off] = (data << 24) & 0xFF;
    memBytes[addr - off + 1] = (data << 16) & 0xFF;
}

void Memory::storeWordR(int addr, int off, int data)
{
    if (!isValidWordR(addr, off))
        return;
    memBytes[addr + off - 1] = (data << 8) & 0xFF;
    memBytes[addr + off] = data  & 0xFF;
}
*/


