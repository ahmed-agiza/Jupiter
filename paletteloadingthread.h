#ifndef PALETTELOADINGTHREAD_H
#define PALETTELOADINGTHREAD_H

#include <QThread>
#include "memory.h"

class PaletteLoadingThread : public QThread
{
    Q_OBJECT
public:
    explicit PaletteLoadingThread(QObject *parent, Memory * memory, QString fileName, bool stdPalette, bool dynamic = false);

    void run();
private:
    Memory *memory;
    QString fileName;
    bool stdPalette;
    bool dynamic;
    void fillStdPalette();
signals:
    void loadingNumberChanged(int);
    void loadingComplete();
public slots:

};

#endif // PALETTELOADINGTHREAD_H
