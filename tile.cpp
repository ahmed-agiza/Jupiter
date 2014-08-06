#include "tile.h"
Tile::Tile()
{
    //texture.setRepeated(true);
    tileData.resize(16);
    tileData.fill(QVector<char>(16,0));
}

Tile::Tile(QVector<QVector<char> > tileData)
{
    this->tileData = tileData;
    //texture.setRepeated(true);
}

void Tile::save()
{
    /*texture.loadFromImage(image);
    for(int i=0; i<spritesPointers.size(); i++) {
        spritesPointers[i]->setTexture(texture);
    }*/
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
    //image.setPixel(col, row, (*pall)[(tileData[row][col])].getColor());
}

/*sf::Texture Tile::getTexture() const
{
    return texture;
}
*/
Tile::~Tile()
{

}
