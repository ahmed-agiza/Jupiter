#ifndef GPUMEMORYDUMP_H
#define GPUMEMORYDUMP_H

#include <QDialog>
#include "memory.h"
#include "loadmemorythread.h"
#include "savememorythread.h"

namespace Ui {
class GpuMemoryDump;
}

class GpuMemoryDump : public QDialog
{
    Q_OBJECT
private slots:
    void on_loadFromFilePushButton_clicked();
    void on_saveToFilePushButton_clicked();

    void on_dumpstdpalette_clicked();

public slots:
    void onNumberChanged(int);
    void complete();
public:
    explicit GpuMemoryDump(QWidget *parent, Memory *memory);
    ~GpuMemoryDump();
    void fillStdPalette();
private:
    Ui::GpuMemoryDump *ui;
    Memory *mem;
    LoadMemoryThread *loadingThread;
    SaveMemoryThread *savingThread;
    int claculateMemorySize(const QVector<bool>&);
};

#endif // GPUMEMORYDUMP_H
