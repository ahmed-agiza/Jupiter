#include "memoryloading.h"
#include "ui_memoryloading.h"

MemoryLoading::MemoryLoading(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MemoryLoading)
{
    ui->setupUi(this);

}

MemoryLoading::~MemoryLoading()
{
    delete ui;
}

void MemoryLoading::on_pushButton_pressed()
{
    mem = new Memory;
    connect(mem, SIGNAL(loadingNumberChanged(int)), this, SLOT(onLoadingNumberChanged(int)));
    myThread = new LoadMemoryThread();
    myThread->memory = mem;
    myThread->start();
}

void MemoryLoading::onLoadingNumberChanged(int number)
{
    ui->progressBar->setValue(number);
}
