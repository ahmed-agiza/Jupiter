#ifndef PALETTECOLOR_H
#define PALETTECOLOR_H
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <QVector>
#include <QSet>

class Tile;
class PaletteColor
{
public:
    PaletteColor();
    sf::Color getColor() const;
    void setColor(sf::Color);
    void addTile(Tile*);
    void storeByte(unsigned int, char);
    char loadByte(unsigned int) const;
private:
    sf::Color color;
    QSet< Tile* > myTilesSet;
    QVector< Tile* > myTiles;
};

#endif // PALETTECOLOR_H
