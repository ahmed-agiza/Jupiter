#include "tilerenderwindow.h"

#define TILE_SIZE 16

TileRenderWindow::TileRenderWindow(QWidget *parent, Memory * memory) :QRenderWindow(parent, QPoint(0,0), QSize(256,256), 20)
{
    this->memory = memory;
    tileIndex = 0;
    tileSetToRender = 0;
    connect(this->memory, SIGNAL(renderNow()), this, SLOT(repaint()));
}

void TileRenderWindow::initialize()
{
    tileToRender.setTexture(memory->backgroundTileSet[tileIndex].getTexture());
    tileToRender.setOrigin(8,8);
    tileToRender.setPosition(127,127);
    scaleFactor = 256 / TILE_SIZE;
    tileToRender.setScale(Vector2f(scaleFactor,scaleFactor));

}

void TileRenderWindow::update()
{
    RenderWindow::clear(Color(47, 47, 47));

    sf::Texture texture;
    if(tileSetToRender == 0)
        texture.loadFromImage(memory->backgroundTileSet[tileIndex].getImage());
    else
        texture.loadFromImage(memory->spritesTileSet[tileIndex].getImage());

    tileToRender.setTexture(texture);
    tileToRender.setScale(Vector2f(scaleFactor,scaleFactor));
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

void TileRenderWindow::setScaleFactor(unsigned int scaleFactor)
{
    this->scaleFactor = scaleFactor;
}

unsigned int TileRenderWindow::getScaleFactor()
{
    return scaleFactor;
}

TileRenderWindow::~TileRenderWindow()
{

}
