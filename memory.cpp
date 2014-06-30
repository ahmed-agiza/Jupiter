#include "memory.h"


memory::memory(int s)
{
    size = s;
    data.resize(s);
}

memory::~memory()
{

}


void memory::storeByte(int addr, int data)
{

}

int memory::loadByte(int addr) const
{

}

int memory::loadByteU(int addr) const
{

}

void memory::storeHWord(int addr, int data)
{

}


int memory::loadHWord(int addr) const
{

}

int memory::loadHWordU(int addr) const
{

}

void memory::storeWord(int addr, int data)
{

}

void memory::storeWordL(int addr, int data)
{

}

void memory::storeWordR(int addr, int data)
{

}

int memory::loadWord(int addr) const
{

}

int memory::loadWordL(int addr) const
{

}

int memory::loadWordR(int addr) const
{

}

int memory::storeConditional(int addr, int data)
{

}

int memory::loadLinked(int addr) const
{

}


