#include "savememorythread.h"

SaveMemoryThread::SaveMemoryThread(QObject *parent, QVector<bool> s) :
    QThread(parent)
{
    segmentsToLoad = s;
}

void SaveMemoryThread::run()
{
    //QMutex mutex;
    qDebug() << "Started!";
    memory->saveMemory(filePath,segmentsToLoad);
    emit saveComplete();
    qDebug() << "Done :)\n";
}
