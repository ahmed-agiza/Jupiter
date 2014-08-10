#include "memoryloading.h"
#include "ui_memoryloading.h"

MemoryLoading::MemoryLoading(QWidget *parent, Memory *mem) :
    QWidget(parent),
    ui(new Ui::MemoryLoading)
{
    ui->setupUi(this);
    this->mem = mem;
}

MemoryLoading::~MemoryLoading()
{
    delete ui;
}

void MemoryLoading::on_pushButton_pressed()
{
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
    engine = new TileEngine(0, QPoint(0,0), QSize(512,384), mem);
    mem->setTileEngine(engine);
    engine->show();
    tileSetViewer = new TileSetViewer(this, mem);
    tileSetViewer->show();
    //tileRenderWindow = new TileRenderWindow(0, mem);
    //tileRenderWindow->show();
}
