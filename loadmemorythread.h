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
    explicit LoadMemoryThread(QObject *parent, QVector<bool>, bool dynamic = false);
    void run();
    Memory* memory;
    QString filePath;
private:
    QVector<bool> segmentsToLoad;
    bool dynamic;
signals:
    void loadComplete();

public slots:

};

#endif // LOADMEMORYTHREAD_H
