#include "paletteviewer.h"
#include "ui_paletteviewer.h"
#include <QGraphicsScene>

PaletteViewer::PaletteViewer(QWidget *parent, Memory * memory) :
    QDialog(parent),
    ui(new Ui::PaletteViewer)
{
    ui->setupUi(this);
    this->memory = memory;

    this->setWindowTitle("Palette Viewer");

    paletteRenderWindow = new PaletteRenderWindow(this,this->memory);
    ui->scrollArea->setWidget(paletteRenderWindow);
    ui->scrollArea->setMouseTracking(true);
    this->setMouseTracking(true);
    paletteRenderWindow->setMouseTracking(true);

    connect(this, SIGNAL(firstTimeRender()), paletteRenderWindow, SLOT(repaint()));
    emit firstTimeRender();
}

PaletteViewer::~PaletteViewer()
{
    delete ui;
}


void PaletteViewer::changeValue(Vector2i mousePosition)
{
    int value = ((mousePosition.y/32) * 16) + (mousePosition.x / 32);

    sf::Color color = memory->palette[value].getColor();
    ui->redValue->setText(QString::number(color.r));
    ui->greenValue->setText(QString::number(color.g));
    ui->blueValue->setText(QString::number(color.b));
    ui->alphaValue->setText(QString::number(color.a));
    ui->colorNumber->setText(QString::number(value));
}

void PaletteViewer::update()
{
    Vector2i position = sf::Mouse::getPosition(*paletteRenderWindow);
    if(position.x >= 0 && position.x < 512 && position.y >= 0 && position.y < 512)
        changeValue(position);
}

void PaletteViewer::mouseMoveEvent(QMouseEvent* event)
{
    update();
}
