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

    for (unsigned int i = verticalScroll / 16; i < ceil((verticalScroll + float(screenSize.y)) / 16.0); i++)
        for (unsigned int j = horizontalScroll / 16; j < ceil((horizontalScroll + float(screenSize.x)) / 16.0); j++){
            Vector2f spritePosition(Vector2f(j * 16 - horizontalScroll, i * 16 - verticalScroll));
            Vector2f spriteOrigin(0,0);

            if(j * 16 < horizontalScroll){
                spritePosition.x = ((j + 1) * 16 - horizontalScroll - 1);
                spriteOrigin.x = 15;
            }
            if(i * 16 < verticalScroll){
                spritePosition.y = ((i + 1) * 16 - verticalScroll - 1);
                spriteOrigin.y = 15;
            }
            memory->backgroundMatrix[i][j].setPosition(spritePosition);
            memory->backgroundMatrix[i][j].setOrigin(spriteOrigin);
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
