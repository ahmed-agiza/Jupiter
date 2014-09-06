#ifndef LOADTILESETSTHREAD_H
#define LOADTILESETSTHREAD_H

#include <QThread>
#include <QVector>
#include <QMap>
#include "memory.h"

class LoadTilesetsThread : public QThread
{
    Q_OBJECT
public:
    explicit LoadTilesetsThread(QObject *parent, Memory *memory, bool tileSetIndex, bool indImages, QString fileName, QStringList *nameList);
    void OpenSingle();
    void OpenMultiple();
    Memory *memory;
    bool tileSetIndex;
    bool indImages;
    QString fileName;
    QStringList *nameList;
    QVector<int> tilesFailed;
    QMap< Uint32, Uint8 > paletteSet;
    sf::Uint8 palettePointer;
    void run();
    sf::Uint32 colorToInt(sf::Color color);
signals:
    void loadComplete();
    void loadingNumberChanged(int);
public slots:

};

#endif // LOADTILESETSTHREAD_H
