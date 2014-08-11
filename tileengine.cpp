#include "tileengine.h"

bool saved = 0;
#define TILE_SIZE 16
TileEngine::TileEngine(QWidget* parent, const QPoint& position, const QSize& size, Memory* mem) : QRenderWindow(parent, position, size, 20)
{
    this->setWindowTitle("Game Screen");
    memory = mem;
    connect(memory, SIGNAL(renderNow()), this, SLOT(repaint()));

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

    for (unsigned int i = verticalScroll / TILE_SIZE; i < ceil((verticalScroll + float(screenSize.y)) / float(TILE_SIZE)); i++)
        for (unsigned int j = horizontalScroll / TILE_SIZE; j < ceil((horizontalScroll + float(screenSize.x)) / float(TILE_SIZE)); j++){
            Vector2f spritePosition(Vector2f(j * TILE_SIZE - horizontalScroll, i * TILE_SIZE - verticalScroll));
            Vector2f spriteOrigin(0,0);

            if(j * TILE_SIZE < horizontalScroll){
                spritePosition.x = ((j + 1) * TILE_SIZE - horizontalScroll - 1);
                spriteOrigin.x = TILE_SIZE - 1;
            }
            if(i * TILE_SIZE < verticalScroll){
                spritePosition.y = ((i + 1) * TILE_SIZE - verticalScroll - 1);
                spriteOrigin.y = TILE_SIZE - 1;
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
