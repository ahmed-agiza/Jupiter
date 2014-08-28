#include "palettecolor.h"
#include "tile.h"

PaletteColor::PaletteColor()
{
    color = Color(0,0,0,0);
}

bool isLitlEndian()
{
    return true;
}

void PaletteColor::storeByte(unsigned int index, char byte)
{
    if(isLitlEndian()){
        if(index == 0)  color.r = byte;
        else if(index == 1)  color.g = byte;
        else if(index == 2)  color.b = byte;
        else if(index == 3)  color.a = byte;
    }else{
        if(index == 3)  color.r = byte;
        else if(index == 2)  color.g = byte;
        else if(index == 1)  color.b = byte;
        else if(index == 0)  color.a = byte;
    }
    // update tiles here
    for(int i=0; i<myTiles.size(); i++){
        myTiles[i]->update();
    }
}

char PaletteColor::loadByte(unsigned int index) const
{
    if(isLitlEndian()){
        if(index == 0)  return color.r;
        else if(index == 1)  return color.g;
        else if(index == 2)  return color.b;
        else if(index == 3)  return color.a;
    }else{
        if(index == 0)  return color.a;
        else if(index == 1)  return color.b;
        else if(index == 2)  return color.g;
        else if(index == 3)  return color.r;
    }
    return 0;
}

sf::Color PaletteColor::getColor() const
{
    return color;
}
void PaletteColor::setColor(sf::Color clr)
{
    color = clr;
}

void PaletteColor::addTile(Tile * tile)
{
    if(!myTilesSet.contains(tile)){
        myTiles.append(tile);
        myTilesSet.insert(tile);
    }
}
