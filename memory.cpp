#include "memory.h"
#include "tileengine.h"

#define TEXT_SEGMENT 1
#define DATA_SEGMENT 2
#define HEAP_SEGMENT 3
#define STACK_SEGMENT 4
#define TILE_MAP 5
#define BG_TILE_SET 6
#define SP_TILE_SET 7
#define SPRITE_RAM 8
#define PALETTE 9
#define OUT_OF_RANGE 0

#define OUT_OF_RANGE_EX_NO 4
#define NOT_HWORD_ALIGN_EX_NO 5
#define NOT_WORD_ALIGN_EX_NO 6

#define TILE_SIZE 16

bool isLittleEndian()
{
    return true;
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
    else if(addr >= stackSegmentLimitAddress - stackSegmentPhysicalSize && addr < stackSegmentLimitAddress)
        return STACK_SEGMENT;
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

Memory::Memory():   textSegmentBaseAddress (0x4000000),
                    dataSegmentBaseAddress (0x10010000),
                    heapSegmentBaseAddress (0x10500000),
                    stackSegmentLimitAddress (0x80000000),
                    spriteRamBaseAddress (0x10400000),
                    paletteBaseAddress (0x104FF000),
                    tileMapBaseAddress (0x10100000),
                    spritesTileSetBaseAddress (0x10030000),
                    backgroundTileSetBaseAddress (0x10020000),
                    textSegmentPhysicalSize (64 * 1024),
                    dataSegmentPhysicalSize (64 * 1024),
                    heapSegmentPhysicalSize (128 * 1024),
                    stackSegmentPhysicalSize (128 * 1024),
                    spriteRamPhysicalSize (512),
                    palettePhysicalSize (1 * 1024),
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
    spriteRam.resize(64);
    palette.resize(256);

    textSegment.resize(textSegmentPhysicalSize);
    dataSegment.resize(dataSegmentPhysicalSize);
    heapSegment.resize(heapSegmentPhysicalSize);
    stackSegment.resize(stackSegmentPhysicalSize);
    textSegment.fill(0);
    dataSegment.fill(0);
    heapSegment.fill(0);
    stackSegment.fill(0);

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
    else if(segment == STACK_SEGMENT)
        stackSegment[addr - (stackSegmentLimitAddress - stackSegmentPhysicalSize)] = data;
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
    else if(segment == STACK_SEGMENT)
        return stackSegment[addr - (stackSegmentLimitAddress - stackSegmentPhysicalSize)];
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
    else return 0;
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
        return ((unsigned short)(loadByte(addr + 1) << 8) & loadByteU(addr));
    else
        return ((unsigned short)(loadByte(addr) << 8) & loadByteU(addr + 1));
}

unsigned short Memory::loadHWordU(unsigned int addr) const
{
    return (unsigned short)loadHWord(addr);
}

void Memory::storeWord(unsigned int addr, int data)
{
    if(getWordSegment(addr) == OUT_OF_RANGE){
        //emit raiseException(OUT_OF_RANGE_EX_NO);
        return;
    }if((addr & 3) != 0){
        //emit raiseException(NOT_WORD_ALIGN_EX_NO);
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
    if(getWordSegment(addr) == OUT_OF_RANGE){
        //emit raiseException(OUT_OF_RANGE_EX_NO);
        return 0;
    }if((addr & 3) != 0){
        //emit raiseException(NOT_WORD_ALIGN_EX_NO);
        return 0;
    }
    if(isLittleEndian()) {
        return (((unsigned int)(loadByte(addr + 3)) << 24) & ((unsigned int)(loadByteU(addr + 2)) << 16) & ((unsigned int)(loadByteU(addr + 1)) << 8) & (loadByteU(addr)));
    } else {
        return (((unsigned int)(loadByte(addr)) << 24) & ((unsigned int)(loadByteU(addr + 1)) << 16) & ((unsigned int)(loadByteU(addr + 2)) << 8) & (loadByteU(addr + 3)));
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
            count++;
        }
    }
    if(segmentsToLoad[1]){
        for(int i=0; i<dataSegmentPhysicalSize; i++){
            in.getChar(byte);
            storeByte(dataSegmentBaseAddress + i, *byte);
            count++;
        }
    }

    if(segmentsToLoad[2]){
        for(int i=0; i<backgroundTileSetPhysicalSize; i++){
            in.getChar(byte);
            storeByte(backgroundTileSetBaseAddress + i, *byte);
            count++;
        }
    }

    if(segmentsToLoad[3]){
        for(int i=0; i<spritesTileSetPhysicalSize; i++){
            in.getChar(byte);
            storeByte(spritesTileSetBaseAddress + i, *byte);
            count++;
        }
    }

    if(segmentsToLoad[4]){
        for(int i=0; i<tileMapPhysicalSize; i++){
            in.getChar(byte);
            storeByte(tileMapBaseAddress + i, *byte);
            count++;
        }
    }

    if(segmentsToLoad[5]){
        for(int i=0; i<spriteRamPhysicalSize; i++){
            in.getChar(byte);
            storeByte(spriteRamBaseAddress + i, *byte);
            count++;
        }
    }

    if(segmentsToLoad[6]){
        for(int i=0; i<palettePhysicalSize; i++){
            in.getChar(byte);
            storeByte(paletteBaseAddress + i, *byte);
            count++;
        }
    }
    if(segmentsToLoad[7]){
        for(int i=0; i<heapSegmentPhysicalSize; i++){
            in.getChar(byte);
            storeByte(heapSegmentBaseAddress + i, *byte);
            count++;
        }
    }


    qDebug() << "loaded: "<<count<<" bytes\n";
    in.close();
    emit loadingNumberChanged(64);
}

void Memory::setTileEngine(TileEngine *engine)
{
    this->engine = engine;
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


