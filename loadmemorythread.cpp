#include "loadmemorythread.h"

LoadMemoryThread::LoadMemoryThread(QObject *parent, QVector<bool> s, bool dynamic) :
    QThread(parent)
{
    segmentsToLoad = s;
    this->dynamic = dynamic;
}

void LoadMemoryThread::run()
{
    //QMutex mutex;
    qDebug() << "Started!";
    memory->loadMemory(filePath,segmentsToLoad,dynamic);
    emit loadComplete();
    qDebug() << "Done :)\n";
}
