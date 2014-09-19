#ifndef TILEMAPLOADINGTHREAD_H
#define TILEMAPLOADINGTHREAD_H

#include <QThread>
#include "memory.h"

class TilemapLoadingThread : public QThread
{
    Q_OBJECT
public:
    explicit TilemapLoadingThread(QObject *parent, Memory *memory, QString fileName, bool dynamic = false);
    void run();
    Memory *memory;
    QString fileName;
    bool dynamic;
signals:
    void loadingNumberChanged(int);
    void loadComplete();
public slots:

};

#endif // TILEMAPLOADINGTHREAD_H
