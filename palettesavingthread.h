#ifndef PALETTESAVINGTHREAD_H
#define PALETTESAVINGTHREAD_H

#include <QThread>
#include "memory.h"

class PaletteSavingThread : public QThread
{
    Q_OBJECT
public:
    explicit PaletteSavingThread(QObject *parent, Memory *memory, QString fileName);
    void run();
    Memory * memory;
    QString fileName;
signals:
    void loadingNumberChanged(int);
    void loadComplete();
public slots:

};

#endif // PALETTESAVINGTHREAD_H
