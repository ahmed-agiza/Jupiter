#ifndef INITIALIZEMEMORYTHREAD_H
#define INITIALIZEMEMORYTHREAD_H

#include <QThread>
#include "memory.h"
class InitializeMemoryThread : public QThread
{
    Q_OBJECT
public:
    explicit InitializeMemoryThread(Memory* memory, QObject *parent = 0);
    void run();
    Memory* memory;
signals:
    void loadComplete();
public slots:

};

#endif // INITIALIZEMEMORYTHREAD_H
