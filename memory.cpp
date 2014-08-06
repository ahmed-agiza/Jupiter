#include "memory.h"

#define TEXT_SEGMENT 1
#define DATA_SEGMENT 2
#define HEAP_SEGMENT 3
#define STACK_SEGMENT 4
#define OUT_OF_RANGE 0

#define outOfRangeExNo 4

bool isLittleEndian()
{
    return true;
}

unsigned int getScreensHeightCount()
{
    return 4;
}

unsigned int getScreensWidthCount()
{
    return 4;
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
    else
        return OUT_OF_RANGE;
        //emit raiseException(outOfRangeExNo);
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
                    textSegmentPhysicalSize (64 * 1024),
                    dataSegmentPhysicalSize (64 * 1024),
                    heapSegmentPhysicalSize (128 * 1024),
                    stackSegmentPhysicalSize (128 * 1024),
                    screenWidth(512),
                    screenHeight(384)
{
    backgroundTileSet.resize(256);
    spritesTileSet.resize(256);
    tileMap.resize(screenHeight/16 * getScreensHeightCount());
    tileMap.fill( QVector<char>(screenWidth/16 * getScreensWidthCount() ));
    textSegment.resize(textSegmentPhysicalSize);
    dataSegment.resize(dataSegmentPhysicalSize);
    heapSegment.resize(heapSegmentPhysicalSize);
    stackSegment.resize(stackSegmentPhysicalSize);
    textSegment.fill(0);
    dataSegment.fill(0);
    heapSegment.fill(0);
    stackSegment.fill(0);
}

Memory::~Memory()
{

}


void Memory::storeByte(unsigned int addr, char data)
{
    int segment = getByteSegment(addr);
    if (segment == OUT_OF_RANGE)
        return;
    else if(segment == TEXT_SEGMENT)
        textSegment[addr - textSegmentBaseAddress] = data;
    else if(segment == DATA_SEGMENT)
        dataSegment[addr - dataSegmentBaseAddress] = data;
    else if(segment == HEAP_SEGMENT)
        heapSegment[addr - heapSegmentBaseAddress] = data;
    else if(segment == STACK_SEGMENT)
        stackSegment[addr - (stackSegmentLimitAddress - stackSegmentPhysicalSize)] = data;
}

char Memory::loadByte(unsigned int addr) const
{
    int segment = getByteSegment(addr);
    if (segment == OUT_OF_RANGE)
        return 0;
    else if(segment == TEXT_SEGMENT)
        return textSegment[addr - textSegmentBaseAddress];
    else if(segment == DATA_SEGMENT)
        return dataSegment[addr - dataSegmentBaseAddress];
    else if(segment == HEAP_SEGMENT)
        return heapSegment[addr - heapSegmentBaseAddress];
    else if(segment == STACK_SEGMENT)
        return stackSegment[addr - (stackSegmentLimitAddress - stackSegmentPhysicalSize)];
    else return 0;
}

unsigned char Memory::loadByteU(unsigned int addr) const
{
    int segment = getByteSegment(addr);
    if (segment == OUT_OF_RANGE)
        return 0;
    else if(segment == TEXT_SEGMENT)
        return (unsigned char)textSegment[addr - textSegmentBaseAddress];
    else if(segment == DATA_SEGMENT)
        return (unsigned char)dataSegment[addr - dataSegmentBaseAddress];
    else if(segment == HEAP_SEGMENT)
        return (unsigned char)heapSegment[addr - heapSegmentBaseAddress];
    else if(segment == STACK_SEGMENT)
        return (unsigned char)stackSegment[addr - (stackSegmentLimitAddress - stackSegmentPhysicalSize)];
    else return 0;
}

void Memory::storeHWord(unsigned int addr, short data)
{
    int segment = getByteSegment(addr);
    if (segment == OUT_OF_RANGE) {
        return;
    } else if(segment == TEXT_SEGMENT) {
        if(isLittleEndian()) {
            textSegment[(addr - textSegmentBaseAddress)] = (data&0xff);
            textSegment[(addr - textSegmentBaseAddress) + 1] = (data>>8);
        } else {
            textSegment[(addr - textSegmentBaseAddress)] = (data>>8);
            textSegment[(addr - textSegmentBaseAddress) + 1] = (data&0xff);
        }
    } else if(segment == DATA_SEGMENT) {
        if(isLittleEndian()) {
            dataSegment[addr - dataSegmentBaseAddress] = (data&0xff);
            dataSegment[addr - dataSegmentBaseAddress + 1] = (data>>8);
        } else {
            dataSegment[addr - dataSegmentBaseAddress] = (data>>8);
            dataSegment[addr - dataSegmentBaseAddress + 1] = (data&0xff);
        }
    } else if(segment == HEAP_SEGMENT) {
        if(isLittleEndian()) {
            heapSegment[addr - heapSegmentBaseAddress] = (data&0xff);
            heapSegment[addr - heapSegmentBaseAddress + 1] = (data>>8);
        } else {
            heapSegment[addr - heapSegmentBaseAddress] = (data>>8);
            heapSegment[addr - heapSegmentBaseAddress + 1] = (data&0xff);
        }
    } else if(segment == STACK_SEGMENT) {
        if(isLittleEndian()) {
            stackSegment[addr - (stackSegmentLimitAddress - stackSegmentPhysicalSize)] = (data&0xff);
            stackSegment[addr - (stackSegmentLimitAddress - stackSegmentPhysicalSize) + 1] = (data>>8);
        } else {
            stackSegment[addr - (stackSegmentLimitAddress - stackSegmentPhysicalSize)] = (data>>8);
            stackSegment[addr - (stackSegmentLimitAddress - stackSegmentPhysicalSize) + 1] = (data&0xff);
        }
    }
}


short Memory::loadHWord(unsigned int addr) const
{
    int segment = getByteSegment(addr);
    if (segment == OUT_OF_RANGE) {
        return 0;
    } else if(segment == TEXT_SEGMENT) {
        if(isLittleEndian()) {
            return ((unsigned short)(textSegment[(addr - textSegmentBaseAddress) + 1] << 8) & textSegment[(addr - textSegmentBaseAddress)]);
        } else {
            return ((unsigned short)(textSegment[(addr - textSegmentBaseAddress)] << 8) & textSegment[(addr - textSegmentBaseAddress) + 1]);
        }
    } else if(segment == DATA_SEGMENT) {
        if(isLittleEndian()) {
            return ((unsigned short)(dataSegment[addr - dataSegmentBaseAddress + 1] << 8) & dataSegment[addr - dataSegmentBaseAddress]);
        } else {
            return ((unsigned short)(dataSegment[addr - dataSegmentBaseAddress] << 8) & dataSegment[addr - dataSegmentBaseAddress + 1]);
        }
    } else if(segment == HEAP_SEGMENT) {
        if(isLittleEndian()) {
            return ((unsigned short)(heapSegment[addr - heapSegmentBaseAddress + 1] << 8) & heapSegment[addr - heapSegmentBaseAddress]);
        } else {
            return ((unsigned short)(heapSegment[addr - heapSegmentBaseAddress] << 8) & heapSegment[addr - heapSegmentBaseAddress + 1]);
        }
    } else if(segment == STACK_SEGMENT) {
        if(isLittleEndian()) {
            return ((unsigned short)(stackSegment[addr - (stackSegmentLimitAddress - stackSegmentPhysicalSize) + 1] << 8) & stackSegment[addr - (stackSegmentLimitAddress - stackSegmentPhysicalSize)]);
        } else {
            return ((unsigned short)(stackSegment[addr - (stackSegmentLimitAddress - stackSegmentPhysicalSize)] << 8) & stackSegment[addr - (stackSegmentLimitAddress - stackSegmentPhysicalSize) + 1]);
        }
    } else return 0;
}

unsigned short Memory::loadHWordU(unsigned int addr) const
{
    int segment = getByteSegment(addr);
    if (segment == OUT_OF_RANGE) {
        return 0;
    } else if(segment == TEXT_SEGMENT) {
        if(isLittleEndian()) {
            return (unsigned short)((unsigned short)(textSegment[(addr - textSegmentBaseAddress) + 1] << 8) & textSegment[(addr - textSegmentBaseAddress)]);
        } else {
            return (unsigned short)((unsigned short)(textSegment[(addr - textSegmentBaseAddress)] << 8) & textSegment[(addr - textSegmentBaseAddress) + 1]);
        }
    } else if(segment == DATA_SEGMENT) {
        if(isLittleEndian()) {
            return (unsigned short)((unsigned short)(dataSegment[addr - dataSegmentBaseAddress + 1] << 8) & dataSegment[addr - dataSegmentBaseAddress]);
        } else {
            return (unsigned short)((unsigned short)(dataSegment[addr - dataSegmentBaseAddress] << 8) & dataSegment[addr - dataSegmentBaseAddress + 1]);
        }
    } else if(segment == HEAP_SEGMENT) {
        if(isLittleEndian()) {
            return (unsigned short)((unsigned short)(heapSegment[addr - heapSegmentBaseAddress + 1] << 8) & heapSegment[addr - heapSegmentBaseAddress]);
        } else {
            return (unsigned short)((unsigned short)(heapSegment[addr - heapSegmentBaseAddress] << 8) & heapSegment[addr - heapSegmentBaseAddress + 1]);
        }
    } else if(segment == STACK_SEGMENT) {
        if(isLittleEndian()) {
            return (unsigned short)((unsigned short)(stackSegment[addr - (stackSegmentLimitAddress - stackSegmentPhysicalSize) + 1] << 8) & stackSegment[addr - (stackSegmentLimitAddress - stackSegmentPhysicalSize)]);
        } else {
            return (unsigned short)((unsigned short)(stackSegment[addr - (stackSegmentLimitAddress - stackSegmentPhysicalSize)] << 8) & stackSegment[addr - (stackSegmentLimitAddress - stackSegmentPhysicalSize) + 1]);
        }
    } else return 0;
}

void Memory::storeWord(unsigned int addr, int data)
{
    int segment = getByteSegment(addr);
    if (segment == OUT_OF_RANGE) {
        return;
    } else if(segment == TEXT_SEGMENT) {
        if(isLittleEndian()) {
            textSegment[(addr - textSegmentBaseAddress)] = (data&0xff);
            textSegment[(addr - textSegmentBaseAddress) + 1] = (data>>8)&0xff;
            textSegment[(addr - textSegmentBaseAddress) + 2] = (data>>16)&0xff;
            textSegment[(addr - textSegmentBaseAddress) + 3] = (data>>24);
        } else {
            textSegment[(addr - textSegmentBaseAddress)] = (data>>24);
            textSegment[(addr - textSegmentBaseAddress) + 1] = (data>>16)&0xff;
            textSegment[(addr - textSegmentBaseAddress) + 2] = (data>>8)&0xff;
            textSegment[(addr - textSegmentBaseAddress) + 3] = (data&0xff);
        }
    } else if(segment == DATA_SEGMENT) {
        if(isLittleEndian()) {
            dataSegment[addr - dataSegmentBaseAddress] = (data&0xff);
            dataSegment[addr - dataSegmentBaseAddress + 1] = (data>>8)&0xff;
            dataSegment[addr - dataSegmentBaseAddress + 2] = (data>>16)&0xff;
            dataSegment[addr - dataSegmentBaseAddress + 3] = (data>>24);
        } else {
            dataSegment[addr - dataSegmentBaseAddress] = (data>>24);
            dataSegment[addr - dataSegmentBaseAddress + 1] = (data>>16)&0xff;
            dataSegment[addr - dataSegmentBaseAddress + 2] = (data>>8)&0xff;
            dataSegment[addr - dataSegmentBaseAddress + 3] = (data&0xff);
        }
    } else if(segment == HEAP_SEGMENT) {
        if(isLittleEndian()) {
            heapSegment[addr - heapSegmentBaseAddress] = (data&0xff);
            heapSegment[addr - heapSegmentBaseAddress + 1] = (data>>8)&0xff;
            heapSegment[addr - heapSegmentBaseAddress + 2] = (data>>16)&0xff;
            heapSegment[addr - heapSegmentBaseAddress + 3] = (data>>24);
        } else {
            heapSegment[addr - heapSegmentBaseAddress] = (data>>24);
            heapSegment[addr - heapSegmentBaseAddress + 1] = (data>>16)&0xff;
            heapSegment[addr - heapSegmentBaseAddress + 2] = (data>>8)&0xff;
            heapSegment[addr - heapSegmentBaseAddress + 3] = (data&0xff);
        }
    } else if(segment == STACK_SEGMENT) {
        if(isLittleEndian()) {
            stackSegment[addr - (stackSegmentLimitAddress - stackSegmentPhysicalSize)] = (data&0xff);
            stackSegment[addr - (stackSegmentLimitAddress - stackSegmentPhysicalSize) + 1] = (data>>8)&0xff;
            stackSegment[addr - (stackSegmentLimitAddress - stackSegmentPhysicalSize) + 2] = (data>>16)&0xff;
            stackSegment[addr - (stackSegmentLimitAddress - stackSegmentPhysicalSize) + 3] = (data>>24);
        } else {
            stackSegment[addr - (stackSegmentLimitAddress - stackSegmentPhysicalSize)] = (data>>24);
            stackSegment[addr - (stackSegmentLimitAddress - stackSegmentPhysicalSize) + 1] = (data>>16)&0xff;
            stackSegment[addr - (stackSegmentLimitAddress - stackSegmentPhysicalSize) + 2] = (data>>8)&0xff;
            stackSegment[addr - (stackSegmentLimitAddress - stackSegmentPhysicalSize) + 3] = (data&0xff);
        }
    }
}

int Memory::loadWord(unsigned int addr) const
{
    int segment = getByteSegment(addr);
    if (segment == OUT_OF_RANGE) {
        return 0;
    } else if(segment == TEXT_SEGMENT) {
        if(isLittleEndian()) {
            return (((unsigned int)(textSegment[(addr - textSegmentBaseAddress) + 3]) << 24) & ((unsigned int)(textSegment[(addr - textSegmentBaseAddress) + 2]) << 16) & ((unsigned int)(textSegment[(addr - textSegmentBaseAddress) + 1]) << 8) & textSegment[(addr - textSegmentBaseAddress)]);
        } else {
            return (((unsigned int)(textSegment[(addr - textSegmentBaseAddress)]) << 24) & ((unsigned int)(textSegment[(addr - textSegmentBaseAddress) + 1]) << 16) & ((unsigned int)(textSegment[(addr - textSegmentBaseAddress) + 2]) << 8) & textSegment[(addr - textSegmentBaseAddress) + 3]);
        }
    } else if(segment == DATA_SEGMENT) {
        if(isLittleEndian()) {
            return (((unsigned int)(dataSegment[addr - dataSegmentBaseAddress + 3]) << 24) & ((unsigned int)(dataSegment[addr - dataSegmentBaseAddress + 2]) << 16) & ((unsigned int)(dataSegment[addr - dataSegmentBaseAddress + 1]) << 8) & dataSegment[addr - dataSegmentBaseAddress]);
        } else {
            return (((unsigned int)(dataSegment[addr - dataSegmentBaseAddress]) << 24) & ((unsigned int)(dataSegment[addr - dataSegmentBaseAddress + 1]) << 16) & ((unsigned int)(dataSegment[addr - dataSegmentBaseAddress + 2]) << 8) & dataSegment[addr - dataSegmentBaseAddress + 3]);
        }
    } else if(segment == HEAP_SEGMENT) {
        if(isLittleEndian()) {
            return (((unsigned int)(heapSegment[addr - heapSegmentBaseAddress + 3]) << 24) & ((unsigned int)(heapSegment[addr - heapSegmentBaseAddress + 2]) << 16) & ((unsigned int)(heapSegment[addr - heapSegmentBaseAddress + 1]) << 8) & heapSegment[addr - heapSegmentBaseAddress]);
        } else {
            return (((unsigned int)(heapSegment[addr - heapSegmentBaseAddress]) << 24) & ((unsigned int)(heapSegment[addr - heapSegmentBaseAddress + 1]) << 16) & ((unsigned int)(heapSegment[addr - heapSegmentBaseAddress + 2]) << 8) & heapSegment[addr - heapSegmentBaseAddress + 3]);
        }
    } else if(segment == STACK_SEGMENT) {
        if(isLittleEndian()) {
            return (((unsigned int)(stackSegment[addr - (stackSegmentLimitAddress - stackSegmentPhysicalSize) + 3]) << 24) & ((unsigned int)(stackSegment[addr - (stackSegmentLimitAddress - stackSegmentPhysicalSize) + 2]) << 16) & ((unsigned int)(stackSegment[addr - (stackSegmentLimitAddress - stackSegmentPhysicalSize) + 1]) << 8) & stackSegment[addr - (stackSegmentLimitAddress - stackSegmentPhysicalSize)]);
        } else {
            return (((unsigned int)(stackSegment[addr - (stackSegmentLimitAddress - stackSegmentPhysicalSize)]) << 24) & ((unsigned int)(stackSegment[addr - (stackSegmentLimitAddress - stackSegmentPhysicalSize) + 1]) << 16) & ((unsigned int)(stackSegment[addr - (stackSegmentLimitAddress - stackSegmentPhysicalSize) + 2]) << 8) & stackSegment[addr - (stackSegmentLimitAddress - stackSegmentPhysicalSize) + 3]);
        }
        if(isLittleEndian()) {
            return ((unsigned int)(stackSegment[addr - (stackSegmentLimitAddress - stackSegmentPhysicalSize) + 1] << 8) & stackSegment[addr - (stackSegmentLimitAddress - stackSegmentPhysicalSize)]);
        } else {
            return ((unsigned int)(stackSegment[addr - (stackSegmentLimitAddress - stackSegmentPhysicalSize)] << 8) & stackSegment[addr - (stackSegmentLimitAddress - stackSegmentPhysicalSize) + 1]);
        }
    } else return 0;
}

unsigned int Memory::loadWordU(unsigned int addr)const
{
    int segment = getByteSegment(addr);
    if (segment == OUT_OF_RANGE) {
        return 0;
    } else if(segment == TEXT_SEGMENT) {
        if(isLittleEndian()) {
            return (unsigned int)(((unsigned int)(textSegment[(addr - textSegmentBaseAddress) + 3]) << 24) & ((unsigned int)(textSegment[(addr - textSegmentBaseAddress) + 2]) << 16) & ((unsigned int)(textSegment[(addr - textSegmentBaseAddress) + 1]) << 8) & textSegment[(addr - textSegmentBaseAddress)]);
        } else {
            return (unsigned int)(((unsigned int)(textSegment[(addr - textSegmentBaseAddress)]) << 24) & ((unsigned int)(textSegment[(addr - textSegmentBaseAddress) + 1]) << 16) & ((unsigned int)(textSegment[(addr - textSegmentBaseAddress) + 2]) << 8) & textSegment[(addr - textSegmentBaseAddress) + 3]);
        }
    } else if(segment == DATA_SEGMENT) {
        if(isLittleEndian()) {
            return (unsigned int)(((unsigned int)(dataSegment[addr - dataSegmentBaseAddress + 3]) << 24) & ((unsigned int)(dataSegment[addr - dataSegmentBaseAddress + 2]) << 16) & ((unsigned int)(dataSegment[addr - dataSegmentBaseAddress + 1]) << 8) & dataSegment[addr - dataSegmentBaseAddress]);
        } else {
            return (unsigned int)(((unsigned int)(dataSegment[addr - dataSegmentBaseAddress]) << 24) & ((unsigned int)(dataSegment[addr - dataSegmentBaseAddress + 1]) << 16) & ((unsigned int)(dataSegment[addr - dataSegmentBaseAddress + 2]) << 8) & dataSegment[addr - dataSegmentBaseAddress + 3]);
        }
    } else if(segment == HEAP_SEGMENT) {
        if(isLittleEndian()) {
            return (unsigned int)(((unsigned int)(heapSegment[addr - heapSegmentBaseAddress + 3]) << 24) & ((unsigned int)(heapSegment[addr - heapSegmentBaseAddress + 2]) << 16) & ((unsigned int)(heapSegment[addr - heapSegmentBaseAddress + 1]) << 8) & heapSegment[addr - heapSegmentBaseAddress]);
        } else {
            return (unsigned int)(((unsigned int)(heapSegment[addr - heapSegmentBaseAddress]) << 24) & ((unsigned int)(heapSegment[addr - heapSegmentBaseAddress + 1]) << 16) & ((unsigned int)(heapSegment[addr - heapSegmentBaseAddress + 2]) << 8) & heapSegment[addr - heapSegmentBaseAddress + 3]);
        }
    } else if(segment == STACK_SEGMENT) {
        if(isLittleEndian()) {
            return (unsigned int)(((unsigned int)(stackSegment[addr - (stackSegmentLimitAddress - stackSegmentPhysicalSize) + 3]) << 24) & ((unsigned int)(stackSegment[addr - (stackSegmentLimitAddress - stackSegmentPhysicalSize) + 2]) << 16) & ((unsigned int)(stackSegment[addr - (stackSegmentLimitAddress - stackSegmentPhysicalSize) + 1]) << 8) & stackSegment[addr - (stackSegmentLimitAddress - stackSegmentPhysicalSize)]);
        } else {
            return (unsigned int)(((unsigned int)(stackSegment[addr - (stackSegmentLimitAddress - stackSegmentPhysicalSize)]) << 24) & ((unsigned int)(stackSegment[addr - (stackSegmentLimitAddress - stackSegmentPhysicalSize) + 1]) << 16) & ((unsigned int)(stackSegment[addr - (stackSegmentLimitAddress - stackSegmentPhysicalSize) + 2]) << 8) & stackSegment[addr - (stackSegmentLimitAddress - stackSegmentPhysicalSize) + 3]);
        }
        if(isLittleEndian()) {
            return (unsigned int)((unsigned int)(stackSegment[addr - (stackSegmentLimitAddress - stackSegmentPhysicalSize) + 1] << 8) & stackSegment[addr - (stackSegmentLimitAddress - stackSegmentPhysicalSize)]);
        } else {
            return (unsigned int)((unsigned int)(stackSegment[addr - (stackSegmentLimitAddress - stackSegmentPhysicalSize)] << 8) & stackSegment[addr - (stackSegmentLimitAddress - stackSegmentPhysicalSize) + 1]);
        }
    } else return 0;
}
void Memory::storeConditional(unsigned int addr, int data)
{
    storeWord(addr, data);
}

int Memory::loadLinked(unsigned int addr) const
{
    return loadWord(addr);
}

/*
bool Memory::isValidWordL(int addr, int off) const
{
    if (addr - off > MemoryLimit)
    {
       // emit raiseException(outOfRangeExNo);
        return false;
    }
    return true;

}
bool Memory::isValidWordR(int addr, int off) const
{
    if (addr + off > MemoryLimit)
    {
        //emit raiseException(outOfRangeExNo);
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


