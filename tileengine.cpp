#include "tileengine.h"

bool saved = 0;

TileEngine::TileEngine(QWidget* parent, const QPoint& position, const QSize& size) : QRenderWindow(parent, position, size, 20)
{

}

void TileEngine::initialize()
{
    screenSize.x = 512;
    screenSize.y = 384;
    verticalScroll = 80;
    horizontalScroll = 88;
}

void TileEngine::update()
{
    RenderWindow::clear(Color(0, 128, 128));
    renderFrame();
}

void TileEngine::setMemory(Memory * memory)
{
    this->memory = memory;
}

void TileEngine::renderFrame()
{

    for (int i = verticalScroll / 16; i < ceil((verticalScroll + float(screenSize.y)) / 16.0); i++)
        for (int j = horizontalScroll / 16; j < ceil((horizontalScroll + float(screenSize.x)) / 16.0); j++){
            memory->backgroundMatrix[i][j].setPosition(Vector2f(j * 16 - horizontalScroll, i * 16 - verticalScroll));
            RenderWindow::draw(memory->backgroundMatrix[i][j]);
        }

    for(int i=0; i<64; i++){
        if(memory->spriteRam[i].shouldRender()){
            RenderWindow::draw(memory->spriteRam[i].sprite);
        }
    }

}

TileEngine::~TileEngine()
{

}
