#include "palettecolor.h"

PaletteColor::PaletteColor()
{
}

void PaletteColor::storeByte(unsigned int index, char byte)
{
    if(index == 0)  color.r = byte;
    else if(index == 1)  color.g = byte;
    else if(index == 2)  color.b = byte;
    else if(index == 3)  color.a = byte;

    // update tiles here
}

char PaletteColor::loadByte(unsigned int index) const
{
    if(index == 0)  return color.r;
    else if(index == 1)  return color.g;
    else if(index == 2)  return color.b;
    else if(index == 3)  return color.a;
    else return 0;
}

sf::Color PaletteColor::getColor() const
{
    return color;
}
void PaletteColor::setColor(sf::Color clr)
{
    color = clr;
}

void PaletteColor::addBgTile(unsigned int tile)
{
    if(!bgTilesSet.contains(tile)) {
        bgTiles.push_back(tile);
        bgTilesSet.insert(tile);
    }
}

void PaletteColor::addSpTile(unsigned int tile)
{
    if(!spTilesSet.contains(tile)) {
        spTiles.push_back(tile);
        spTilesSet.insert(tile);
    }
}

unsigned int PaletteColor::getBgTile(unsigned int index) const
{
    return bgTiles[index];
}
unsigned int PaletteColor::getSpTile(unsigned int index) const
{
    return spTiles[index];
}
unsigned int PaletteColor::getBgTilesCount() const
{
    return bgTiles.size();
}
unsigned int PaletteColor::getSpTilesCount() const
{
    return spTiles.size();
}
