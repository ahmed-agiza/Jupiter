#include "loadmemorythread.h"

LoadMemoryThread::LoadMemoryThread(QObject *parent, QVector<bool> s) :
    QThread(parent)
{
    segmentsToLoad = s;
}

void LoadMemoryThread::run()
{
    //QMutex mutex;
    qDebug() << "Started!";
    memory->loadMemory(":/memory/memory.bin",segmentsToLoad);
    qDebug() << "Done :)\n";
}
