#include "gamesprite.h"

bool isLilEndian()
{
    return true;
}

GameSprite::GameSprite()
{
    image.create(32,32);
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
    case 1:
        img0 = (*tileSetPointer)[memoryBytes[0]].getImage();
        img1 = (*tileSetPointer)[memoryBytes[1]].getImage();
        makeImage();
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
        img0 = (*tileSetPointer)[memoryBytes[0]].getImage();
        img1 = (*tileSetPointer)[memoryBytes[1]].getImage();
        makeImage();
        break;
    }

}

void GameSprite::makeImage()
{
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
    if(renderingEnabled) {
        if(tile0Rotation == 1) {
            Image tempImg = img0;
            for(int i=0; i<16; i++)
                for(int j=0; j<16; j++)
                    img0.setPixel(j,i,tempImg.getPixel(15-i,j));
        }
        else if(tile0Rotation == 2) {
            Image tempImg = img0;
            for(int i=0; i<16; i++)
                for(int j=0; j<16; j++)
                    img0.setPixel(j,i,tempImg.getPixel(i,15-j));
        }
        else if(tile0Rotation == 3) {
            Image tempImg = img0;
            for(int i=0; i<16; i++)
                for(int j=0; j<16; j++)
                    img0.setPixel(j,i,tempImg.getPixel(15-i,15-j));
        }
        if(tile0FlipV)
            img0.flipVertically();
        if(tile0FlipH)
            img0.flipHorizontally();
        for(int i=0; i<16; i++)
            for(int j=0; j<16; j++)
                image.setPixel(j,i,img0.getPixel(j,i));
        if(twoSprites) {
            if(tile1Rotation == 1) {
                Image tempImg = img1;
                for(int i=0; i<16; i++)
                    for(int j=0; j<16; j++)
                        img1.setPixel(j,i,tempImg.getPixel(15-i,j));
            }
            else if(tile1Rotation == 2) {
                Image tempImg = img1;
                for(int i=0; i<16; i++)
                    for(int j=0; j<16; j++)
                        img1.setPixel(j,i,tempImg.getPixel(i,15-j));
            }
            else if(tile1Rotation == 3) {
                Image tempImg = img1;
                for(int i=0; i<16; i++)
                    for(int j=0; j<16; j++)
                        img1.setPixel(j,i,tempImg.getPixel(15-i,15-j));
            }
            if(tile1FlipV)
                img1.flipVertically();
            if(tile1FlipH)
                img1.flipHorizontally();
            if(attachedHorizontally) {
                for(int i=0; i<16; i++)
                    for(int j=16; j<32; j++)
                        image.setPixel(j,i,img1.getPixel(j-16,i));
                texture.loadFromImage(image);
                sprite.setTexture(texture);
                sprite.setTextureRect(IntRect(0,0,32,16));
            } else {
                for(int i=16; i<32; i++)
                    for(int j=0; j<16; j++)
                        image.setPixel(j,i,img1.getPixel(j,i-16));
                texture.loadFromImage(image);
                sprite.setTexture(texture);
                sprite.setTextureRect(IntRect(0,0,16,32));
            }
        } else {
            texture.loadFromImage(img0);
            sprite.setTexture(texture);
            sprite.setTextureRect(IntRect(0,0,16,16));
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
    img0 = (*tileSetPointer)[memoryBytes[0]].getImage();
    img1 = (*tileSetPointer)[memoryBytes[1]].getImage();
    makeImage();
}
