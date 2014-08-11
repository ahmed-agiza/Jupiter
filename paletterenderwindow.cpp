#include "paletterenderwindow.h"

PaletteRenderWindow::PaletteRenderWindow(QWidget *parent, Memory * memory):QRenderWindow(parent, QPoint(0,0), QSize(110,110), 20)
{
    this->memory = memory;
    this->paletteIndex = 0;
    connect(this->memory, SIGNAL(renderNow()), this, SLOT(repaint()));
    color = Color(0,0,0);
}

PaletteRenderWindow::~PaletteRenderWindow()
{

}

void PaletteRenderWindow::initialize()
{
    color = memory->palette[paletteIndex].getColor();
}

void PaletteRenderWindow::update()
{
    color = memory->palette[paletteIndex].getColor();
    RenderWindow::clear(color);
}

void PaletteRenderWindow::setPaletteIndex(unsigned char index)
{
    paletteIndex = index;
}

unsigned char PaletteRenderWindow::getPaletteIndex()
{
    return paletteIndex;
}
