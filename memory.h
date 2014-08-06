#ifndef MEMORY_H
#define MEMORY_H

#include <QMap>
#include <QByteArray>
#include <QObject>
#include <QMatrix>
#include <QVector>
#include <SFML/Graphics.hpp>
#include "palettecolor.h"
#include "tile.h"
#include "gamesprite.h"

class Memory : public QObject
{
    Q_OBJECT
public:
    Memory ();
    void storeByte(unsigned int, char);
    char loadByte(unsigned int) const;
    unsigned char loadByteU(unsigned int) const;

    void storeHWord(unsigned int, short);
    short loadHWord(unsigned int) const;
    unsigned short loadHWordU(unsigned int) const;

    void storeWord(unsigned int, int);
    int loadWord(unsigned int) const;
    unsigned int loadWordU(unsigned int) const;

    void storeConditional(unsigned int, int);
    int loadLinked(unsigned int) const;

    unsigned int getByteSegment(unsigned int) const;
    unsigned int getHWordSegment(unsigned int) const;
    unsigned int getWordSegment(unsigned int) const;

//    bool isValidWordL(int, int) const;
//    bool isValidWordR(int, int) const;
//    void storeWordL(int, int, int);
//    void storeWordR(int, int, int);
//    int loadWordL(int, int) const;
//    int loadWordR(int, int) const;

    ~Memory();

signals:
    void raiseException(int);

private:
    QByteArray textSegment;
    QByteArray dataSegment;
    QByteArray heapSegment;
    QByteArray stackSegment;
    QVector< Tile > backgroundTileSet;
    QVector< Tile > spritesTileSet;
    QVector< QVector< char > > tileMap;
    QVector< QVector< sf::Sprite > > backgroundMatrix;
    QVector< PaletteColor > palette;
    QVector< GameSprite > spriteRam;
    const unsigned int textSegmentBaseAddress;
    const unsigned int dataSegmentBaseAddress;
    const unsigned int heapSegmentBaseAddress;
    const unsigned int stackSegmentLimitAddress;

    const unsigned int textSegmentPhysicalSize;
    const unsigned int dataSegmentPhysicalSize;
    const unsigned int heapSegmentPhysicalSize;
    const unsigned int stackSegmentPhysicalSize;

    const unsigned int screenWidth;
    const unsigned int screenHeight;
};

#endif // MEMORY_H
