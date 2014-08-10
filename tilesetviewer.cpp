#include "tilesetviewer.h"
#include "ui_tilesetviewer.h"
#include <QGridLayout>

TileSetViewer::TileSetViewer(QWidget *parent, Memory * memory) :
    QDialog(parent),
    ui(new Ui::TileSetViewer)
{
    ui->setupUi(this);
    this->memory = memory;


    tileRenderWindow = new TileRenderWindow(0,this->memory);
    ui->scrollArea->setWidget(tileRenderWindow);
}

TileSetViewer::~TileSetViewer()
{
    delete ui;
}
