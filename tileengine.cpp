#include <QDebug>
#include "tileengine.h"

#define TILE_SIZE 16

inline int ceilDev(int x, int y)
{
    return (x + y - 1)/y;
}

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
    RenderWindow::clear(Color(0, 128, 128));
    renderFrame();
    //RenderWindow::display();
}

void TileEngine::setMemory(Memory * memory)
{
    this->memory = memory;
}

void TileEngine::renderFrame()
{
    qDebug() << "started";
    for (unsigned int i = (*verticalScroll) / TILE_SIZE; i < ceilDev(*verticalScroll + screenSize.y, TILE_SIZE); i++)
        for (unsigned int j = (*horizontalScroll) / TILE_SIZE; j <  ceilDev(*horizontalScroll + screenSize.x, TILE_SIZE); j++){
            RenderWindow::draw(memory->backgroundMatrix[i][j]);
        }

    for(int priority=0; priority<4; priority++)
        for(int i=0; i<64; i++){
            if(memory->spriteRam[i].shouldRender() && memory->spriteRam[i].getPriority() == priority){
                RenderWindow::draw(memory->spriteRam[i].sprite);
            }
        }
    qDebug() << "ended";

}

TileEngine::~TileEngine()
{

}
