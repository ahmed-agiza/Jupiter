#include "tilemapsavingthread.h"

TilemapSavingThread::TilemapSavingThread(QObject *parent, Memory *memory, QString fileName) :
    QThread(parent)
{
    this->fileName = fileName;
    this->memory = memory;
}

void TilemapSavingThread::run()
{
    if(fileName.endsWith(".mtmap")){

    }else{

    }
}
