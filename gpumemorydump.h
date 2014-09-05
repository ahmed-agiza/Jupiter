#ifndef GPUMEMORYDUMP_H
#define GPUMEMORYDUMP_H

#include <QDialog>

namespace Ui {
class GpuMemoryDump;
}

class GpuMemoryDump : public QDialog
{
    Q_OBJECT

public:
    explicit GpuMemoryDump(QWidget *parent = 0);
    ~GpuMemoryDump();

private:
    Ui::GpuMemoryDump *ui;
};

#endif // GPUMEMORYDUMP_H
