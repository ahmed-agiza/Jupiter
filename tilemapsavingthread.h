#ifndef TILEMAPSAVINGTHREAD_H
#define TILEMAPSAVINGTHREAD_H

#include <QThread>
#include "memory.h"

class TilemapSavingThread : public QThread
{
    Q_OBJECT
public:
    explicit TilemapSavingThread(QObject *parent, Memory *memory, QString fileName);
    void run();
    Memory *memory;
    QString fileName;
signals:
    void loadingNumberChanged(int);
    void loadComplete();
public slots:
};

#endif // TILEMAPSAVINGTHREAD_H
