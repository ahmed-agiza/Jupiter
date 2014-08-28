#ifndef MEMORYLOADING_H
#define MEMORYLOADING_H

#include <QWidget>
#include <QFrame>
#include "memory.h"
#include "loadmemorythread.h"
#include "tileengine.h"
#include "tilesetviewer.h"
#include "tilerenderwindow.h"
namespace Ui {
class MemoryLoading;
}

class MemoryLoading : public QDialog
{
    Q_OBJECT

public:
    explicit MemoryLoading(QDialog *parent, Memory*);
    bool isInit();
    ~MemoryLoading();
signals:
    void firstRender();
private slots:
    void on_pushButton_pressed();
    void onLoadingNumberChanged(int);
    void loadComplete();
private:
    bool initialized;
    Ui::MemoryLoading *ui;
    Memory* mem;
    LoadMemoryThread* myThread;
    TileEngine *engine;
    QFrame* MainFrame;
    TileSetViewer *tileSetViewer;
    TileRenderWindow *tileRenderWindow;

    int claculateMemorySize(const QVector<bool> &segments);
};

#endif // MEMORYLOADING_H
