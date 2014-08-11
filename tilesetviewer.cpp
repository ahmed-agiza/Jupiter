#include "tilesetviewer.h"
#include "ui_tilesetviewer.h"
#include <QGridLayout>

TileSetViewer::TileSetViewer(QWidget *parent, Memory * memory) :
    QDialog(parent),
    ui(new Ui::TileSetViewer)
{
    ui->setupUi(this);
    this->memory = memory;

    this->setWindowTitle("Tileset Viewer");

    tileRenderWindow = new TileRenderWindow(this,this->memory);
    ui->scrollArea->setWidget(tileRenderWindow);
    connect(this, SIGNAL(renderScreen()), tileRenderWindow, SLOT(repaint()));
}

TileSetViewer::~TileSetViewer()
{
    delete ui;
}


void TileSetViewer::on_pushButton_clicked()
{
    if(tileRenderWindow->getTileSetToRender() == 0){
        ui->label_2->setText("Currently Viewing: \n    Sprites Tileset");
        ui->pushButton->setText("Switch to \nBackground Tileset");
        tileRenderWindow->setTileSetToRender(1);
        emit renderScreen();
    } else {
        ui->label_2->setText("Currently Viewing: \n    Background Tileset");
        ui->pushButton->setText("Switch to \nSprites Tileset");
        tileRenderWindow->setTileSetToRender(0);
        emit renderScreen();
    }
}

void TileSetViewer::on_horizontalSlider_valueChanged(int value)
{
    tileRenderWindow->setTileIndex(value);
    emit renderScreen();
    ui->label->setText(QString::number(tileRenderWindow->getTileIndex()));
}
