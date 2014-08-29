#include "gamesprite.h"
#include <QDebug>

#define TILE_SIZE 16
bool isLilEndian()
{
    return true;
}

GameSprite::GameSprite()
{
    image.create(TILE_SIZE * 2, TILE_SIZE * 2);
    memoryBytes.resize(8);
    memoryBytes.fill(0);
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
        if(memoryBytes[0] != memoryBytes[1])
            (*tileSetPointer)[memoryBytes[0]].removeGameSprite(this);
        makeImage();
        (*tileSetPointer)[memoryBytes[0]].addGameSprite(this);
        break;
    case 1:
        if(memoryBytes[0] != memoryBytes[1])
            (*tileSetPointer)[memoryBytes[1]].removeGameSprite(this);
        makeImage();
        (*tileSetPointer)[memoryBytes[1]].addGameSprite(this);
        break;
    case 2:
    case 3:
    case 4:
    case 5:
        if (isLilEndian())
            sprite.setPosition(((unsigned int)memoryBytes[3]<<8)|memoryBytes[2], ((unsigned int)memoryBytes[5]<<8)|memoryBytes[4]);
        else
            sprite.setPosition(((unsigned int)memoryBytes[2]<<8)|memoryBytes[3], ((unsigned int)memoryBytes[4]<<8)|memoryBytes[5]);
        break;
    case 6:
    case 7:
        makeImage();
        break;
    }

}

void GameSprite::makeImage()
{
    img0 = (*tileSetPointer)[memoryBytes[0]].getImage();
    img1 = (*tileSetPointer)[memoryBytes[1]].getImage();

    bool renderingEnabled = (memoryBytes[7]>>7)&1;
    bool twoSprites = (memoryBytes[7])&1;
    bool attachedHorizontally = (memoryBytes[7]>>1)&1;
    int tile0Rotation = memoryBytes[6]&3;
    bool tile0FlipV = (memoryBytes[6]>>2)&1;
    bool tile0FlipH = (memoryBytes[6]>>3)&1;
    int tile1Rotation = (memoryBytes[6]>>4)&3;
    bool tile1FlipV = (memoryBytes[6]>>6)&1;
    bool tile1FlipH = (memoryBytes[6]>>7)&1;
    bool scaleH = (memoryBytes[7]>>2)&1;
    bool scaleV = (memoryBytes[7]>>3)&1;
    if(1/*renderingEnabled*/) {
        if(tile0Rotation == 1) {
            Image tempImg = img0;
            for(int i=0; i<TILE_SIZE; i++)
                for(int j=0; j<TILE_SIZE; j++)
                    img0.setPixel(j,i,tempImg.getPixel(TILE_SIZE-1-i,j));
        }
        else if(tile0Rotation == 2) {
            Image tempImg = img0;
            for(int i=0; i<TILE_SIZE; i++)
                for(int j=0; j<TILE_SIZE; j++)
                    img0.setPixel(j,i,tempImg.getPixel(i,TILE_SIZE-1-j));
        }
        else if(tile0Rotation == 3) {
            Image tempImg = img0;
            for(int i=0; i<TILE_SIZE; i++)
                for(int j=0; j<TILE_SIZE; j++)
                    img0.setPixel(j,i,tempImg.getPixel(TILE_SIZE-1-j,TILE_SIZE-1-i));
        }
        if(tile0FlipV)
            img0.flipVertically();
        if(tile0FlipH)
            img0.flipHorizontally();

        for(int i=0; i<TILE_SIZE; i++)
            for(int j=0; j<TILE_SIZE; j++)
                image.setPixel(j,i,img0.getPixel(j,i));

        if(twoSprites) {
            if(tile1Rotation == 1) {
                Image tempImg = img1;
                for(int i=0; i<TILE_SIZE; i++)
                    for(int j=0; j<TILE_SIZE; j++)
                        img1.setPixel(j,i,tempImg.getPixel(TILE_SIZE-1-i,j));
            }
            else if(tile1Rotation == 2) {
                Image tempImg = img1;
                for(int i=0; i<TILE_SIZE; i++)
                    for(int j=0; j<TILE_SIZE; j++)
                        img1.setPixel(j,i,tempImg.getPixel(i,TILE_SIZE-1-j));
            }
            else if(tile1Rotation == 3) {
                Image tempImg = img1;
                for(int i=0; i<TILE_SIZE; i++)
                    for(int j=0; j<TILE_SIZE; j++)
                        img1.setPixel(j,i,tempImg.getPixel(TILE_SIZE-1-j,TILE_SIZE-1-i));
            }
            if(tile1FlipV)
                img1.flipVertically();
            if(tile1FlipH)
                img1.flipHorizontally();
            if(attachedHorizontally) {
                for(int i=0; i<TILE_SIZE; i++)
                    for(int j=TILE_SIZE; j<(TILE_SIZE * 2); j++)
                        image.setPixel(j,i,img1.getPixel(j-TILE_SIZE,i));
                texture.loadFromImage(image);
                sprite.setTexture(texture);
                sprite.setTextureRect(IntRect(0,0,TILE_SIZE * 2,TILE_SIZE));
            } else {
                for(int i=TILE_SIZE; i<TILE_SIZE * 2; i++)
                    for(int j=0; j<TILE_SIZE; j++)
                        image.setPixel(j,i,img1.getPixel(j,i-TILE_SIZE));
                texture.loadFromImage(image);
                sprite.setTexture(texture);
                sprite.setTextureRect(IntRect(0,0,TILE_SIZE,TILE_SIZE * 2));
            }
        } else {
            texture.loadFromImage(img0);
            sprite.setTexture(texture);
            sprite.setTextureRect(IntRect(0,0,TILE_SIZE,TILE_SIZE));
        }
        sprite.setScale(scaleH + 1, scaleV + 1);
    }
}

bool GameSprite::shouldRender() const
{
    return ((memoryBytes[7]>>7)&1);
}

void GameSprite::setTileSet(QVector<Tile>* tileSet)
{
    this->tileSetPointer = tileSet;
    makeImage();
}

int GameSprite::getPriority()
{
    return ((memoryBytes[7]>>4)&0x3);
}

bool GameSprite::isTwoTiles()
{
    return (memoryBytes[7])&1;
}

void GameSprite::setSpriteSize(bool size)
{
    if(size)
        memoryBytes[7] |= (unsigned char)(1);
    else
        memoryBytes[7] &= (~(unsigned char)(1));
    makeImage();
}

bool GameSprite::isAttachedHorizontally()
{
    return (memoryBytes[7]>>1)&1;
}

void GameSprite::setAttachementOrientation(bool o)
{
    if(o)
        memoryBytes[7] |= (unsigned char)(1<<1);
    else
        memoryBytes[7] &= (~(unsigned char)(1<<1));
    makeImage();
}

void GameSprite::setRenderingMode(bool mode)
{
    if(mode)
        memoryBytes[7] |= (unsigned char)(1<<7);
    else
        memoryBytes[7] &= (~(unsigned char)(1<<7));
}

void GameSprite::setTileIndex(bool tileNumber, sf::Uint8 index)
{
    memoryBytes[tileNumber] = index;
    if(tileNumber == 0){
        if(memoryBytes[0] != memoryBytes[1])
            (*tileSetPointer)[memoryBytes[0]].removeGameSprite(this);
        makeImage();
        (*tileSetPointer)[memoryBytes[0]].addGameSprite(this);
    }else{
        if(memoryBytes[0] != memoryBytes[1])
            (*tileSetPointer)[memoryBytes[1]].removeGameSprite(this);
        makeImage();
        (*tileSetPointer)[memoryBytes[1]].addGameSprite(this);
    }
}

sf::Uint8 GameSprite::getTileIndex(bool tileNumber)
{
    return memoryBytes[tileNumber];
}

bool GameSprite::getTile0FlipX()
{
    return (memoryBytes[6]>>3)&1;
}

bool GameSprite::getTile1FlipX()
{
    return (memoryBytes[6]>>7)&1;
}

void GameSprite::setTile0FlipX(bool value)
{
    if(value){
        memoryBytes[6] |= (sf::Uint8(1)<<3);
    }else{
        memoryBytes[6] &= (~(sf::Uint8(1)<<3));
    }
    makeImage();
}

void GameSprite::setTile1FlipX(bool value)
{
    if(value){
        memoryBytes[6] |= (sf::Uint8(1)<<7);
    }else{
        memoryBytes[6] &= (~(sf::Uint8(1)<<7));
    }
    makeImage();
}

bool GameSprite::getTile0FlipY()
{
    return (memoryBytes[6]>>2)&1;
}

bool GameSprite::getTile1FlipY()
{
    return (memoryBytes[6]>>6)&1;
}

void GameSprite::setTile0FlipY(bool value)
{
    if(value){
        memoryBytes[6] |= (sf::Uint8(1)<<2);
    }else{
        memoryBytes[6] &= (~(sf::Uint8(1)<<2));
    }
    makeImage();
}

void GameSprite::setTile1FlipY(bool value)
{
    if(value){
        memoryBytes[6] |= (sf::Uint8(1)<<6);
    }else{
        memoryBytes[6] &= (~(sf::Uint8(1)<<6));
    }
    makeImage();
}

void GameSprite::setLayer(int layer)
{
    if(layer&1){
        memoryBytes[7] |= (sf::Uint8(1)<<4);
    }else{
        memoryBytes[7] &= (~(sf::Uint8(1)<<4));
    }
    if((layer>>1)&1){
        memoryBytes[7] |= (sf::Uint8(1)<<5);
    }else{
        memoryBytes[7] &= (~(sf::Uint8(1)<<5));
    }
}

int GameSprite::getLayer()
{
    return ((memoryBytes[7]>>4) &0x3);
}

bool GameSprite::getScaleX()
{
    return (memoryBytes[7]>>2)&1;
}

bool GameSprite::getScaleY()
{
    return (memoryBytes[7]>>3)&1;
}

void GameSprite::setScaleX(bool value)
{
    if(value){
        memoryBytes[7] |= (sf::Uint8(1)<<2);
    }else{
        memoryBytes[7] &= (~(sf::Uint8(1)<<2));
    }
    makeImage();
}

void GameSprite::setScaleY(bool value)
{
    if(value){
        memoryBytes[7] |= (sf::Uint8(1)<<3);
    }else{
        memoryBytes[7] &= (~(sf::Uint8(1)<<3));
    }
    makeImage();
}

void GameSprite::setPositionX(int position)
{
    if(isLilEndian()){
        memoryBytes[2] = position&0xff;
        memoryBytes[3] = (position>>8)&0xff;
        sprite.setPosition(((unsigned int)memoryBytes[3]<<8)|memoryBytes[2], ((unsigned int)memoryBytes[5]<<8)|memoryBytes[4]);
    }else{
        memoryBytes[3] = position&0xff;
        memoryBytes[2] = (position>>8)&0xff;
        sprite.setPosition(((unsigned int)memoryBytes[2]<<8)|memoryBytes[3], ((unsigned int)memoryBytes[4]<<8)|memoryBytes[5]);
    }
}

void GameSprite::setPositionY(int position)
{
    if(isLilEndian()){
        memoryBytes[4] = position&0xff;
        memoryBytes[5] = (position>>8)&0xff;
        sprite.setPosition(((unsigned int)memoryBytes[3]<<8)|memoryBytes[2], ((unsigned int)memoryBytes[5]<<8)|memoryBytes[4]);
    }else{
        memoryBytes[5] = position&0xff;
        memoryBytes[4] = (position>>8)&0xff;
        sprite.setPosition(((unsigned int)memoryBytes[2]<<8)|memoryBytes[3], ((unsigned int)memoryBytes[4]<<8)|memoryBytes[5]);
    }
}

int GameSprite::getPositionX()
{
    if (isLilEndian())
        return (((unsigned int)memoryBytes[3]<<8)|memoryBytes[2]);
    else
        return (((unsigned int)memoryBytes[2]<<8)|memoryBytes[3]);
}

int GameSprite::getPositionY()
{
    if (isLilEndian())
        return (((unsigned int)memoryBytes[5]<<8)|memoryBytes[4]);
    else
        return (((unsigned int)memoryBytes[4]<<8)|memoryBytes[5]);
}

int GameSprite::getTile0Rotation()
{
    return (memoryBytes[6]&3);
}

int GameSprite::getTile1Rotation()
{
    return ((memoryBytes[6]>>4)&3);
}

void GameSprite::setTile0Rotation(int value)
{
    if(value&1){
        memoryBytes[6] |= sf::Uint8(1);
    }else{
        memoryBytes[6] &= (~sf::Uint8(1));
    }
    if((value>>1)&1){
        memoryBytes[6] |= (sf::Uint8(1)<<1);
    }else{
        memoryBytes[6] &= (~(sf::Uint8(1)<<1));
    }
    makeImage();
}

void GameSprite::setTile1Rotation(int value)
{
    if(value&1){
        memoryBytes[6] |= (sf::Uint8(1)<<4);
    }else{
        memoryBytes[6] &= (~(sf::Uint8(1)<<4));
    }
    if((value>>1)&1){
        memoryBytes[6] |= (sf::Uint8(1)<<5);
    }else{
        memoryBytes[6] &= (~(sf::Uint8(1)<<5));
    }
    makeImage();
}
