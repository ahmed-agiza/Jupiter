#include "tilerenderwindow.h"

#define TILE_SIZE 16

TileRenderWindow::TileRenderWindow(QWidget *parent, Memory * memory) :QRenderWindow(parent, QPoint(0,0), QSize(256,256), 20)
{
    this->memory = memory;
    connect(this->memory, SIGNAL(renderNow()), this, SLOT(repaint()));
}

void TileRenderWindow::initialize()
{
    tileIndex = 0;
    tileSetToRender = 0;
    tileToRender.setScale(256 / TILE_SIZE, 256 / TILE_SIZE);
    tileToRender.setTexture(memory->backgroundTileSet[tileIndex].getTexture());
}

void TileRenderWindow::update()
{
    RenderWindow::clear(Color(0, 128, 128));

    sf::Texture texture;
    if(tileSetToRender == 0)
        texture.loadFromImage(memory->backgroundTileSet[tileIndex].getImage());
    else
        texture.loadFromImage(memory->backgroundTileSet[tileIndex].getImage());

    tileToRender.setTexture(texture);
    RenderWindow::draw(tileToRender);
}

void TileRenderWindow::setTileSetToRender(bool tileSetToRender)
{
    this->tileSetToRender = tileSetToRender;
}

void TileRenderWindow::setTileIndex(unsigned char tileIndex)
{
    this->tileIndex = tileIndex;
}

bool TileRenderWindow::getTileSetToRender()
{
    return tileSetToRender;
}

unsigned char TileRenderWindow::getTileIndex()
{
    return tileIndex;
}

TileRenderWindow::~TileRenderWindow()
{

}
