#include "savetilesetthread.h"
#include "mainwindow.h"

SaveTilesetThread::SaveTilesetThread(QObject *parent, Memory *memory, bool tileSetIndex, bool indImages, QString fileName) :
    QThread(parent)
{
    this->memory = memory;
    this->tileSetIndex = tileSetIndex;
    this->indImages = indImages;
    this->fileName = fileName;
}

void SaveTilesetThread::saveSingle()
{
    QVector<Tile> *tileSet = &(tileSetIndex? memory->spritesTileSet:memory->backgroundTileSet);
    sf::Image tilesetimg;
    tilesetimg.create(256,256);
    for(int i=0; i<16; i++){
        for(int j=0; j<16; j++){
            tilesetimg.copy((*tileSet)[i*16+j].getImage(),j*16,i*16,IntRect(0,0,16,16));
            emit loadingNumberChanged(i*16+j);
        }
    }
    tilesetimg.saveToFile(fileName.toStdString());
    emit loadComplete();
}

void SaveTilesetThread::saveMultiple()
{
    QString filePath = "D:/tiles/"; // Project Folder
    QVector<Tile> *tileSet = &(tileSetIndex? memory->spritesTileSet:memory->backgroundTileSet);
    for(int i=0; i<256; i++){
        (*tileSet)[i].getImage().saveToFile((filePath+QString::number(i)+".png").toStdString());
        emit loadingNumberChanged(i);
    }
    emit loadComplete();
}

void SaveTilesetThread::run()
{
    if(indImages)
        saveMultiple();
    else
        saveSingle();
}
