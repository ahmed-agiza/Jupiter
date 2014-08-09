#ifndef MEMORYLOADING_H
#define MEMORYLOADING_H

#include <QWidget>
#include <QFrame>
#include "memory.h"
#include "loadmemorythread.h"
#include "tileengine.h"
namespace Ui {
class MemoryLoading;
}

class MemoryLoading : public QWidget
{
    Q_OBJECT

public:
    explicit MemoryLoading(QWidget *parent = 0);
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

};

#endif // MEMORYLOADING_H
