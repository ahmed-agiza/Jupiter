#include "tile.h"


Tile::Tile()
{
    texture.setRepeated(true);
    tileData.resize(16);
    tileData.fill(QVector<char>(16,0));
    image.create(16,16,Color(0,0,0,255));
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
    for(int i=0; i<16; i++)
        for(int j=0; j<16; j++)
            image.setPixel(j, i, (*pall)[(tileData[i][j])].getColor());
    save();
}

void Tile::addSprite(Sprite* sp)
{
    spritesPointers.append(sp);
}

void Tile::removeSprite(Sprite* sp)
{
    for(int i=0; i<spritesPointers.size(); i++)
        if(spritesPointers[i] == sp){
            spritesPointers.remove(i);
            i = spritesPointers.size();
        }
}
