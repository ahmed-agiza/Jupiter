#ifndef LOADMEMORYTHREAD_H
#define LOADMEMORYTHREAD_H

#include <QThread>
#include <QtCore>
#include <QVector>
#include <QMutex>
#include "memory.h"

class LoadMemoryThread : public QThread
{
    Q_OBJECT
public:
    explicit LoadMemoryThread(QObject *parent, QVector<bool>);
    void run();
    Memory* memory;
private:
    QVector<bool> segmentsToLoad;
signals:

public slots:

};

#endif // LOADMEMORYTHREAD_H
