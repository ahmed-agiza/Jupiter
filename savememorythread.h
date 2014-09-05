#ifndef SAVEMEMORYTHREAD_H
#define SAVEMEMORYTHREAD_H

#include <QThread>
#include <QtCore>
#include <QVector>
#include <QMutex>
#include "memory.h"

class SaveMemoryThread : public QThread
{
    Q_OBJECT
public:
    explicit SaveMemoryThread(QObject *parent, QVector<bool>);
    void run();
    Memory* memory;
    QString filePath;
private:
    QVector<bool> segmentsToLoad;
signals:
    void saveComplete();

public slots:

};

#endif // SAVEMEMORYTHREAD_H
