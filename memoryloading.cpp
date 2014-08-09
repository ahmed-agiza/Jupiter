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
    //engine = new TileEngine(this, QPoint(0,0), QSize(512,384));
    //mem->setTileEngine(engine);
    connect(mem, SIGNAL(loadingNumberChanged(int)), this, SLOT(onLoadingNumberChanged(int)));
    myThread = new LoadMemoryThread();
    myThread->memory = mem;
    myThread->start();

}

void MemoryLoading::onLoadingNumberChanged(int number)
{
    this->resize(552, 424);
    engine = new TileEngine(0, QPoint(0,0), QSize(512,384));
    mem->setTileEngine(engine);
    engine->setMemory(mem);
    engine->show();
}
