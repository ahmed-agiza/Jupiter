#ifndef GAMESPRITE_H
#define GAMESPRITE_H
#include <QVector>
#include <SFML/Graphics.hpp>
#include "tile.h"
using namespace sf;

class GameSprite
{
public:
    GameSprite();
    ~GameSprite();
    char loadByte(unsigned int) const;
    void storeByte(unsigned int, char);
private:
    QVector<Tile> *tileSetPointer;
    Image img0, img1;
    Image image;
    Texture texture;
    Sprite sprite;
    QVector<unsigned char> memoryBytes;

    void makeImage();
};

#endif // GAMESPRITE_H
