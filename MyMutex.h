#ifndef MYMUTEX_H
#define MYMUTEX_H
#include <QAtomicInt>

class MyMutex
{
    QAtomicInt x;

public:
    MyMutex()
    {
        x = 0;
    }

    void lock()
    {
        while(x == 1);
        x = 1;
    }

    void unlock()
    {
        x=0;
    }
};

class MyLockGuard
{
    MyMutex* m;

public:

    MyLockGuard(MyMutex* x)
    {
        m = x;
        m->lock();
    }

    ~MyLockGuard()
    {
        m->unlock();
    }
};

#endif // MYMUTEX_H
