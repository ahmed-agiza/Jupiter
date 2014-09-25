#include <QDebug>
#include "tileengine.h"

bool saved = 0;
#define TILE_SIZE 16
TileEngine::TileEngine(QWidget* parent, const QPoint& position, const QSize& size, Memory* mem, QVector<int> *mRegisters) : QRenderWindow(parent, position, size, 20)
{
    setWindowTitle("Game Screen");
    memory = mem;
    verticalScroll = &((*mRegisters)[25]);
    horizontalScroll = &((*mRegisters)[24]);
    QObject::connect(memory, SIGNAL(renderNow()), this, SLOT(repaint()));
}

void TileEngine::initialize()
{
    screenSize.x = 512;
    screenSize.y = 384;
}

void TileEngine::update()
{
    qDebug() << "render now";
    RenderWindow::clear(Color(0, 128, 128));
    renderFrame();
}

void TileEngine::setMemory(Memory * memory)
{
    this->memory = memory;
}

void TileEngine::renderFrame()
{

    for (unsigned int i = (*verticalScroll) / TILE_SIZE; i < ceil(((*verticalScroll) + float(screenSize.y)) / float(TILE_SIZE)); i++)
        for (unsigned int j = (*horizontalScroll) / TILE_SIZE; j < ceil(((*horizontalScroll) + float(screenSize.x)) / float(TILE_SIZE)); j++){
            Vector2f spritePosition(Vector2f(j * TILE_SIZE - (*horizontalScroll), i * TILE_SIZE - (*verticalScroll)));
            Vector2f spriteOrigin(0,0);
            if(j * TILE_SIZE < (*horizontalScroll)){
                spritePosition.x = ((j + 1) * TILE_SIZE - (*horizontalScroll) - 1);
                spriteOrigin.x = TILE_SIZE - 1;
            }
            if(i * TILE_SIZE < (*verticalScroll)){
                spritePosition.y = ((i + 1) * TILE_SIZE - (*verticalScroll) - 1);
                spriteOrigin.y = TILE_SIZE - 1;
            }
           // memory->backgroundMatrix[i][j].setPosition(spritePosition);
            //memory->backgroundMatrix[i][j].setOrigin(spriteOrigin);

            emit setSpritePosition(i, j, spritePosition);
            emit setSpriteOrigin(i, j, spriteOrigin);

            RenderWindow::draw(memory->backgroundMatrix[i][j]);

        }

    for(int priority=0; priority<4; priority++)
        for(int i=0; i<64; i++){
            if(memory->spriteRam[i].shouldRender() && memory->spriteRam[i].getPriority() == priority){
                RenderWindow::draw(memory->spriteRam[i].sprite);
            }
        }
}

TileEngine::~TileEngine()
{

}
