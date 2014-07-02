#include "memory.h"

#define outOfRangeExNo 4

bool memory::isValidByte(int addr) const
{
    if(addr >= memBytes.size())
    {
        //emit raiseException(outOfRangeExNo);
        return false;
    }
    return true;
}
bool memory::isValidHWord(int addr) const
{
    if(addr + 1 >= memBytes.size())
    {
        //emit raiseException(outOfRangeExNo);
        return false;
    }
    return true;

}
bool memory::isValidWord(int addr) const
{
   if(addr + 3 >= memBytes.size())
    {
       //emit raiseException(outOfRangeExNo);
       return false;
   }
    return true;

}
bool memory::isValidWordL(int addr, int off) const
{
    if (addr - off >= memBytes.size())
    {
       // emit raiseException(outOfRangeExNo);
        return false;
    }
    return true;

}
bool memory::isValidWordR(int addr, int off) const
{
    if (addr + off >= memBytes.size())
    {
        //emit raiseException(outOfRangeExNo);
        return false;
    }
    return true;

}



memory::memory(int s)
{
    size = s;
    memBytes.resize(s);
    memBytes.fill(0);
}

memory::~memory()
{

}


void memory::storeByte(int addr, int data)
{
   if (!isValidByte(addr))
       return;
   memBytes[addr] = (data & 0xFF);
}

int memory::loadByte(int addr) const
{
    if (!isValidByte(addr))
        return 0;
    return memBytes[addr];
}

unsigned int memory::loadByteU(int addr) const
{
    if (!isValidByte(addr))
        return 0;
    return (unsigned int)memBytes[addr];
}

void memory::storeHWord(int addr, int data)
{
    if (!isValidHWord(addr))
        return;
    memBytes[addr] = (data << 8) & 0xFF;
    memBytes[addr + 1] = (char) data & 0xFF;
}


int memory::loadHWord(int addr) const
{
    if (!isValidHWord(addr))
        return 0;
    return (int)((memBytes[addr] << 16) | memBytes[addr+1]);
}

unsigned int memory::loadHWordU(int addr) const
{
    if (!isValidHWord(addr))
        return 0;
    return (unsigned int)((memBytes[addr] << 16) | memBytes[addr+1]);
}

void memory::storeWord(int addr, int data)
{
    if (!isValidWord(addr))
        return;
    memBytes[addr] = (data << 24) & 0xFF;
    memBytes[addr + 1] = (data << 16) & 0xFF;
    memBytes[addr + 2] = (data << 8) & 0xFF;
    memBytes[addr + 3] = data & 0xFF;
}

void memory::storeWordL(int addr, int off, int data)
{
    if (!isValidWordL(addr, off))
        return;
    memBytes[addr - off] = (data << 24) & 0xFF;
    memBytes[addr - off + 1] = (data << 16) & 0xFF;
}

void memory::storeWordR(int addr, int off, int data)
{
    if (!isValidWordR(addr, off))
        return;
    memBytes[addr + off - 1] = (data << 8) & 0xFF;
    memBytes[addr + off] = data  & 0xFF;
}

int memory::loadWord(int addr) const
{
    if (!isValidWord(addr))
        return 0;
    return (int)((memBytes[addr] << 24) | (memBytes[addr + 1] << 16) | (memBytes[addr + 2] << 8) | (memBytes[addr + 3]));
}

unsigned int memory::loadWordU(int addr)const
{
    if (!isValidWord(addr))
        return 0;
    return (unsigned int)((memBytes[addr] << 24) | (memBytes[addr + 1] << 16) | (memBytes[addr + 2] << 8) | (memBytes[addr + 3]));
}

int memory::loadWordL(int addr, int off) const
{
    if (!isValidWordL(addr, off))
        return 0;
    return (int) (memBytes[addr - off] << 8 |  memBytes[addr - off + 1]);
}

int memory::loadWordR(int addr, int off) const
{
    if (!isValidWordR(addr, off))
        return 0;
    return (int) (memBytes[addr + off] << 8 |  memBytes[addr + off - 1] << 8);
}

void memory::storeConditional(int addr, int data)
{
    if (!isValidWord(addr))
        return;
    storeWord(addr, data);
}

int memory::loadLinked(int addr) const
{
    if (!isValidWord(addr))
        return 0;
    loadWord(addr);
}


