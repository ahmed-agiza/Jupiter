#ifndef TILE_H
#define TILE_H
#include <SFML/Graphics.hpp>
#include <QVector>
#include "palettecolor.h"

using namespace sf;
class Tile
{
public:
    Tile();
    Tile(QVector<QVector<char> >);
    ~Tile();
    void save();
    char loadByte(unsigned int) const;
    void storeByte(unsigned int, char);
    Texture getTexture() const;
    Image getImage() const;
    void update();
    void addSprite(Sprite*);
    void removeSprite(Sprite*);
private:
    QVector<PaletteColor>* pall;
    Image image;
    QVector<QVector<char> > tileData;
    Texture texture;
    QVector< Sprite* > spritesPointers;
};

#endif // TILE_H
