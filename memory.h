#ifndef MEMORY_H
#define MEMORY_H

#include <QMap>
#include <QByteArray>
#include <QObject>
#include <QMatrix>
#include <QVector>
#include <QString>
#include <QFile>
#include <QDebug>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <fstream>
#include "palettecolor.h"
#include "tile.h"
#include "gamesprite.h"

class TileEngine;
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

    void loadMemory(QString, QVector<bool>);
    void saveMemory(QString, QVector<bool>);
//    bool isValidWordL(int, int) const;
//    bool isValidWordR(int, int) const;
//    void storeWordL(int, int, int);
//    void storeWordR(int, int, int);
//    int loadWordL(int, int) const;
//    int loadWordR(int, int) const;

    ~Memory();

    QVector< QVector< sf::Sprite > > backgroundMatrix;
    QVector< GameSprite > spriteRam;
    QVector< Tile > backgroundTileSet;
    QVector< Tile > spritesTileSet;
    QVector< QVector< char > > tileMap;
    QVector< PaletteColor > palette;

    void setTileEngine(TileEngine *);
signals:
    void raiseException(int);
    void loadingNumberChanged(int);
    void renderNow();
private:
    QByteArray textSegment;
    QByteArray dataSegment;
    QByteArray heapSegment;
    QByteArray stackSegment;

    TileEngine* engine;
    int claculateLoadSize(const QVector<bool>&);
public:
    const unsigned int textSegmentBaseAddress;
    const unsigned int dataSegmentBaseAddress;
    const unsigned int heapSegmentBaseAddress;
    const unsigned int stackSegmentLimitAddress;

    const unsigned int spriteRamBaseAddress;
    const unsigned int paletteBaseAddress;
    const unsigned int tileMapBaseAddress;
    const unsigned int spritesTileSetBaseAddress;
    const unsigned int backgroundTileSetBaseAddress;

    const unsigned int textSegmentPhysicalSize;
    const unsigned int dataSegmentPhysicalSize;
    const unsigned int heapSegmentPhysicalSize;
    const unsigned int stackSegmentPhysicalSize;

    const unsigned int spriteRamPhysicalSize;
    unsigned int palettePhysicalSize;
    unsigned int tileMapPhysicalSize;
    unsigned int spritesTileSetPhysicalSize;
    unsigned int backgroundTileSetPhysicalSize;

    const unsigned int screenWidth;
    const unsigned int screenHeight;
};

#endif // MEMORY_H
