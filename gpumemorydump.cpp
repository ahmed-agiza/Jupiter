#include "gpumemorydump.h"
#include "ui_gpumemorydump.h"

GpuMemoryDump::GpuMemoryDump(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GpuMemoryDump)
{
    ui->setupUi(this);
}

GpuMemoryDump::~GpuMemoryDump()
{
    delete ui;
}
