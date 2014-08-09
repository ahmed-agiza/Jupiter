#include "tile.h"
#include "gamesprite.h"

#define TILE_SIZE 16
Tile::Tile()
{
    texture.setRepeated(true);
    tileData.resize(TILE_SIZE);
    tileData.fill(QVector<char>(TILE_SIZE,0));
    image.create(TILE_SIZE,TILE_SIZE,Color(0,0,0,255));
}

void Tile::setPalette(QVector<PaletteColor> *palette)
{
    this->pall = palette;
}

Tile::Tile(QVector<QVector<char> > tileData)
{
    this->tileData = tileData;
    texture.setRepeated(true);
}

void Tile::save()
{
    texture.loadFromImage(image);
    for(int i=0; i<spritesPointers.size(); i++) {
        spritesPointers[i]->setTexture(texture);
    }
    for(int i=0; i<gameSpritesPointers.size(); i++) {
        gameSpritesPointers[i]->makeImage();
    }
}

char Tile::loadByte(unsigned int address) const
{
    unsigned char row = (address&0xf0)>>4;
    unsigned char col = (address&0xf);
    return tileData[row][col];
}
void Tile::storeByte(unsigned int address, char byte)
{
    unsigned char row = (address&0xf0)>>4;
    unsigned char col = (address&0xf);

    tileData[row][col] = byte;
    image.setPixel(col, row, (*pall)[(tileData[row][col])].getColor());
    (*pall)[(tileData[row][col])].addTile(this);
    save();
}

sf::Texture Tile::getTexture() const
{
    return texture;
}

sf::Image Tile::getImage() const
{
    return image;
}
Tile::~Tile()
{

}

void Tile::update()
{
    for(int i=0; i<TILE_SIZE; i++)
        for(int j=0; j<TILE_SIZE; j++)
            image.setPixel(j, i, (*pall)[(tileData[i][j])].getColor());
    save();
}

void Tile::addSprite(Sprite *sp)
{
    if(!spritesPointersSet.contains(sp)){
        spritesPointers.append(sp);
        spritesPointersSet.insert(sp);
    }
}

void Tile::removeSprite(Sprite* sp)
{
    for(int i=0; i<spritesPointers.size(); i++)
        if(spritesPointers[i] == sp){
            spritesPointers.remove(i);
            i = spritesPointers.size();
        }
}

void Tile::addGameSprite(GameSprite *sp)
{
    if(!gameSpritesPointersSet.contains(sp)){
        gameSpritesPointers.append(sp);
        gameSpritesPointersSet.insert(sp);
    }
}

void Tile::removeGameSprite(GameSprite* sp)
{
    if(gameSpritesPointersSet.contains(sp))
    {
        gameSpritesPointersSet.remove(sp);
        for(int i=0; i<gameSpritesPointers.size(); i++)
            if(gameSpritesPointers[i] == sp){
                gameSpritesPointers.remove(i);
                i = gameSpritesPointers.size();
            }
    }
}
