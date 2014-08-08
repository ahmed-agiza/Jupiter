#ifndef LOADMEMORYTHREAD_H
#define LOADMEMORYTHREAD_H

#include <QThread>
#include <QtCore>
#include <QMutex>
#include "memory.h"

class LoadMemoryThread : public QThread
{
    Q_OBJECT
public:
    explicit LoadMemoryThread(QObject *parent = 0);
    void run();
    Memory* memory;
signals:

public slots:

};

#endif // LOADMEMORYTHREAD_H
