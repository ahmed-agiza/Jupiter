#include "memoryloading.h"
#include "ui_memoryloading.h"

MemoryLoading::MemoryLoading(QWidget *parent, Memory *mem) :
    QWidget(parent),
    ui(new Ui::MemoryLoading)
{
    ui->setupUi(this);
    this->mem = mem;
    initialized = false;
}

bool MemoryLoading::isInit()
{
    return initialized;
}

MemoryLoading::~MemoryLoading()
{
    delete ui;
}

int MemoryLoading::claculateMemorySize(const QVector<bool>& segments)
{
    int totalSize = 0;
    if(segments[0])
        totalSize += mem->textSegmentPhysicalSize;
    if(segments[1])
        totalSize += mem->dataSegmentPhysicalSize;
    if(segments[2])
        totalSize += mem->backgroundTileSetPhysicalSize;
    if(segments[3])
        totalSize += mem->spritesTileSetPhysicalSize;
    if(segments[4])
        totalSize += mem->tileMapPhysicalSize;
    if(segments[5])
        totalSize += mem->spriteRamPhysicalSize;
    if(segments[6])
        totalSize += mem->palettePhysicalSize;

    return totalSize;
}

void MemoryLoading::on_pushButton_pressed()
{
    connect(mem, SIGNAL(loadingNumberChanged(int)), this, SLOT(onLoadingNumberChanged(int)));
    ui->pushButton->setEnabled(false);
    QVector<bool> segmentsToLoad(8);
    segmentsToLoad.fill(0);
    segmentsToLoad[2] = 1;
    segmentsToLoad[3] = 1;
    segmentsToLoad[4] = 1;
    segmentsToLoad[5] = 1;
    segmentsToLoad[6] = 1;
    ui->progressBar->setMaximum((claculateMemorySize(segmentsToLoad) + 1024 - 1)/1024);
    myThread = new LoadMemoryThread(this,segmentsToLoad);
    myThread->memory = mem;
    QObject::connect(myThread, SIGNAL(loadComplete()), this, SLOT(loadComplete()));
    myThread->start();

}

void MemoryLoading::onLoadingNumberChanged(int number)
{
    ui->progressBar->setValue(number);
}

void MemoryLoading::loadComplete()
{
    initialized = true;
    this->hide();
}
