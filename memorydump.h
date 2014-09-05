#ifndef MEMORYDUMP_H
#define MEMORYDUMP_H

#include <QDialog>
#include "memory.h"
#include "loadmemorythread.h"
#include "savememorythread.h"

namespace Ui {
class MemoryDump;
}

class MemoryDump : public QDialog
{
    Q_OBJECT

public:
    explicit MemoryDump(QWidget *parent, Memory *mem);
    ~MemoryDump();

private slots:
    void on_loadFromFilePushButton_clicked();

    void on_saveToFilePushButton_clicked();

public slots:
    void onNumberChanged(int);
    void complete();
private:
    Ui::MemoryDump *ui;
    int claculateMemorySize(const QVector<bool>& segments);
    Memory *mem;
    LoadMemoryThread *loadingThread;
    SaveMemoryThread *savingThread;
};

#endif // MEMORYDUMP_H
