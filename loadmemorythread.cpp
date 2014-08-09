#include "loadmemorythread.h"

LoadMemoryThread::LoadMemoryThread(QObject *parent) :
    QThread(parent)
{

}

void LoadMemoryThread::run()
{
    //QMutex mutex;
    qDebug() << "Started!";
    QVector<bool> segmentsToLoad(8);
    segmentsToLoad.fill(0);
    segmentsToLoad[2] = 1;
    segmentsToLoad[3] = 1;
    segmentsToLoad[4] = 1;
    segmentsToLoad[5] = 1;
    segmentsToLoad[6] = 1;
    memory->loadMemory("C:/memory.bin",segmentsToLoad);
    qDebug() << "Done :)\n";
}
