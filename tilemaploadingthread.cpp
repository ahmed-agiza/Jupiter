#include "tilemaploadingthread.h"

TilemapLoadingThread::TilemapLoadingThread(QObject *parent, Memory *memory, QString fileName) :
    QThread(parent)
{
    this->fileName = fileName;
    this->memory = memory;
}

void TilemapLoadingThread::run()
{
    if(fileName.endsWith(".mtmap")){

    }else{

    }
}
