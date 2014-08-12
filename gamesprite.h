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
    bool shouldRender() const;
    void setTileSet(QVector<Tile>*);
    Image img0, img1;
    Image image;
    Sprite sprite;
    QVector<unsigned char> memoryBytes;
    void makeImage();
    int getPriority();
private:
    QVector<Tile> *tileSetPointer;
    Texture texture;

};

#endif // GAMESPRITE_H
