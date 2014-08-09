#ifndef TILE_H
#define TILE_H
#include <SFML/Graphics.hpp>
#include <QVector>
#include <QSet>
#include "palettecolor.h"

using namespace sf;

class GameSprite;
class Tile
{
public:
    Tile();
    Tile(QVector<QVector<char> >);
    ~Tile();
    void setPalette(QVector<PaletteColor> *);
    void save();
    char loadByte(unsigned int) const;
    void storeByte(unsigned int, char);
    Texture getTexture() const;
    Image getImage() const;
    void update();
    void addGameSprite(GameSprite *);
    void removeGameSprite(GameSprite *);
    void addSprite(Sprite *);
    void removeSprite(Sprite *);
private:
    QVector<PaletteColor>* pall;
    Image image;
    QVector<QVector<char> > tileData;
    Texture texture;
    QSet< GameSprite* > gameSpritesPointersSet;
    QVector< GameSprite* > gameSpritesPointers;
    QSet< Sprite* > spritesPointersSet;
    QVector< Sprite* > spritesPointers;
};

#endif // TILE_H
