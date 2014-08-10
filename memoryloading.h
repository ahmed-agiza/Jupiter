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

class MemoryLoading : public QWidget
{
    Q_OBJECT

public:
    explicit MemoryLoading(QWidget *parent, Memory*);
    ~MemoryLoading();
signals:
    void firstRender();
private slots:
    void on_pushButton_pressed();
    void onLoadingNumberChanged(int);
private:
    Ui::MemoryLoading *ui;
    Memory* mem;
    LoadMemoryThread* myThread;
    TileEngine *engine;
    QFrame* MainFrame;
    TileSetViewer *tileSetViewer;
    TileRenderWindow *tileRenderWindow;
};

#endif // MEMORYLOADING_H
