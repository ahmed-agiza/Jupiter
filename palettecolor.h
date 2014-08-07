#ifndef PALETTECOLOR_H
#define PALETTECOLOR_H
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <QVector>
#include <QSet>
class PaletteColor
{
public:
    PaletteColor();
    sf::Color getColor() const;
    void setColor(sf::Color);
    void addBgTile(unsigned int);
    void addSpTile(unsigned int);
    unsigned int getBgTile(unsigned int) const;
    unsigned int getSpTile(unsigned int) const;
    unsigned int getBgTilesCount() const;
    unsigned int getSpTilesCount() const;
    void storeByte(unsigned int, char);
    char loadByte(unsigned int) const;
private:
    sf::Color color;
    QSet<unsigned int>bgTilesSet;
    QSet<unsigned int>spTilesSet;
    QVector<unsigned int> bgTiles;
    QVector<unsigned int> spTiles;
};

#endif // PALETTECOLOR_H
