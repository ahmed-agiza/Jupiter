#include "tileengine.h"



TileEngine::TileEngine(QWidget* parent, const QPoint& position, const QSize& size) : QRenderWindow(parent, position, size)
{

}

void TileEngine::initialize()
{
    framesPerSecond = 25;
    time = milliseconds(1000.0/double(framesPerSecond));
    screenSize.x = 512;
    screenSize.y = 384;
    clk.restart();
}

void TileEngine::update()
{
    RenderWindow::clear(Color(0, 128, 128));
    renderFrame();
    QRenderWindow::paintEvent(0);
}

void TileEngine::renderFrame()
{
    for (int i = verticalScroll / 16; i < ceil((verticalScroll + float(screenSize.y)) / 16); i++)
        for (int j = horizontalScroll / 16; j < ceil((horizontalScroll + float(screenSize.x)) / 16); j++){
            memory->backgroundMatrix[i][j].setPosition(Vector2f(j * 16 - horizontalScroll, i * 16 - verticalScroll));
            RenderWindow::draw(memory->backgroundMatrix[i][j]);
        }

    for(int i=0; i<64; i++){
        if(memory->spriteRam[i].shouldRender())
            RenderWindow::draw(memory->spriteRam[i].sprite);
    }
}

TileEngine::~TileEngine()
{

}
