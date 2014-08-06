#include "gamesprite.h"

GameSprite::GameSprite()
{
    //image.create(32,32);
    memoryBytes.resize(8);
}

GameSprite::~GameSprite()
{

}

char GameSprite::loadByte(unsigned int address) const
{
    return memoryBytes[address&0x7];
}

void GameSprite::storeByte(unsigned int address, char byte)
{
    int index = address&0x7;
    memoryBytes[index] = byte;
    switch(index) {
    case 0:
        break;
    case 1:
        break;
    case 2:
        break;
    case 3:
        break;
    case 4:
        break;
    case 5:
        break;
    case 6:
        break;
    case 7:
        break;
    }
}
