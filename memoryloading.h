#ifndef MEMORYLOADING_H
#define MEMORYLOADING_H

#include <QWidget>
#include "memory.h"
#include "loadmemorythread.h"
namespace Ui {
class MemoryLoading;
}

class MemoryLoading : public QWidget
{
    Q_OBJECT

public:
    explicit MemoryLoading(QWidget *parent = 0);
    ~MemoryLoading();

private slots:
    void on_pushButton_pressed();
    void onLoadingNumberChanged(int);
private:
    Ui::MemoryLoading *ui;
    Memory* mem;
    LoadMemoryThread* myThread;

};

#endif // MEMORYLOADING_H
