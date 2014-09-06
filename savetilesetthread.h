#ifndef SAVETILESETTHREAD_H
#define SAVETILESETTHREAD_H

#include <QThread>
#include <QVector>
#include <QMap>
#include "memory.h"

class SaveTilesetThread : public QThread
{
    Q_OBJECT
public:
    explicit SaveTilesetThread(QObject *parent, Memory *memory, bool tileSetIndex, bool indImages, QString fileName);
    void saveSingle();
    void saveMultiple();
    void run();
    Memory *memory;
    bool tileSetIndex;
    bool indImages;
    QString fileName;
signals:
    void loadComplete();
    void loadingNumberChanged(int);

public slots:

};

#endif // SAVETILESETTHREAD_H
