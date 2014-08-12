#include "paletterenderwindow.h"

PaletteRenderWindow::PaletteRenderWindow(QWidget *parent, Memory * memory):QRenderWindow(parent, QPoint(0,0), QSize(512,512), 20)
{
    this->memory = memory;

    screen.resize(16);
    screen.fill(QVector<sf::Sprite>(16));
    for(int i=0; i<16; i++)
        for(int j=0; j<16; j++){
            screen[i][j].setPosition(j * 32, i * 32);
        }
    connect(this->memory, SIGNAL(renderNow()), this, SLOT(repaint()));
}

PaletteRenderWindow::~PaletteRenderWindow()
{

}

void PaletteRenderWindow::initialize()
{
    RenderWindow::clear();
    for(int i=0; i<16; i++)
        for(int j=0; j<16; j++){
            sf::Image img;
            img.create(32,32,memory->palette[i*16+j].getColor());
            sf::Texture txt;
            txt.loadFromImage(img);
            screen[i][j].setTexture(txt);
            RenderWindow::draw(screen[i][j]);
        }
}

void PaletteRenderWindow::update()
{
    RenderWindow::clear();
    for(int i=0; i<16; i++)
        for(int j=0; j<16; j++){
            sf::Image img;
            img.create(32,32,memory->palette[i*16+j].getColor());
            sf::Texture txt;
            txt.loadFromImage(img);
            screen[i][j].setTexture(txt);
            RenderWindow::draw(screen[i][j]);
        }
}

