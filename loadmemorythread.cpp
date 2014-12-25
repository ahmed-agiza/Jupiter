#include "loadmemorythread.h"

LoadMemoryThread::LoadMemoryThread(QObject *parent, bool dynamic) :
    QThread(parent)
{
    this->dynamic = dynamic;
}

void LoadMemoryThread::run()
{
    if(!dynamic)
        memory->loadMemory(filePath,segmentsToLoad);
    else
        memory->readTilemapToFile(filePath);
    emit loadComplete();
    qDebug() << "\n";

    for(int i=0; i<memory->dynamicOutFileList.size(); i++)
    {
        qDebug() << memory->dynamicOutFileList[i];
    }
}
