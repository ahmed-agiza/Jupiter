#include "palettecolor.h"

PaletteColor::PaletteColor()
{
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
