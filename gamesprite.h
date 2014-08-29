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
    bool isAttachedHorizontally();
    bool isTwoTiles();
    void setSpriteSize(bool);
    void setAttachementOrientation(bool);
    void setRenderingMode(bool mode);
    void setTileIndex(bool tileNumber, sf::Uint8 index);
    sf::Uint8 getTileIndex(bool tileNumber);
    bool getTile0FlipX();
    bool getTile1FlipX();
    void setTile0FlipX(bool);
    void setTile1FlipX(bool);
    bool getTile0FlipY();
    bool getTile1FlipY();
    void setTile0FlipY(bool);
    void setTile1FlipY(bool);
    void setLayer(int);
    int getLayer();
    bool getScaleX();
    bool getScaleY();
    void setScaleX(bool value);
    void setScaleY(bool value);
    void setPositionX(int position);
    void setPositionY(int position);
    int getPositionX();
    int getPositionY();
    int getTile0Rotation();
    int getTile1Rotation();
    void setTile0Rotation(int value);
    void setTile1Rotation(int value);
private:
    QVector<Tile> *tileSetPointer;
    Texture texture;

};

#endif // GAMESPRITE_H
