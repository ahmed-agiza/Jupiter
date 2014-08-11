#include "paletteviewer.h"
#include "ui_paletteviewer.h"

PaletteViewer::PaletteViewer(QWidget *parent, Memory * memory) :
    QDialog(parent),
    ui(new Ui::PaletteViewer)
{
    ui->setupUi(this);
    this->memory = memory;

    this->setWindowTitle("Palette Viewer");

    paletteRenderWindow = new PaletteRenderWindow(this,this->memory);
    ui->scrollArea->setWidget(paletteRenderWindow);
    connect(this, SIGNAL(renderScreen()), paletteRenderWindow, SLOT(repaint()));

    changeValue(0);
}

PaletteViewer::~PaletteViewer()
{
    delete ui;
}

void PaletteViewer::on_horizontalSlider_valueChanged(int value)
{
    changeValue(value);
}

void PaletteViewer::changeValue(int value)
{
    paletteRenderWindow->setPaletteIndex(value);
    ui->redValue->setText(QString::number(memory->palette[value].getColor().r));
    ui->greenValue->setText(QString::number(memory->palette[value].getColor().g));
    ui->blueValue->setText(QString::number(memory->palette[value].getColor().b));
    ui->alphaValue->setText(QString::number(memory->palette[value].getColor().a));
    ui->colorNumber->setText(QString::number(value));
    emit renderScreen();
}
