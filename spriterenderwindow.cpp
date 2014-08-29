#include "spriterenderwindow.h"

#define TILE_SIZE 16
SpriteRenderWindow::SpriteRenderWindow(QWidget *parent, Memory* memory) :
    QRenderWindow(parent, QPoint(0,0), QSize(256,256), 20)
{
    this->memory = memory;
    spriteIndex = 0;
    connect(this->memory, SIGNAL(renderNow()), this, SLOT(repaint()));
}

void SpriteRenderWindow::initialize()
{
    spriteToRender = memory->spriteRam[spriteIndex];
    if(spriteToRender.isTwoTiles()){
        if(spriteToRender.isAttachedHorizontally()){
            spriteToRender.sprite.setOrigin(16,8);
        }else{
            spriteToRender.sprite.setOrigin(8,16);
        }
    }else{
        spriteToRender.sprite.setOrigin(8,8);
    }
    spriteToRender.sprite.setPosition(127,127);
    scaleFactor = 128 / TILE_SIZE;
    spriteToRender.sprite.setScale(Vector2f(scaleFactor,scaleFactor));

}

void SpriteRenderWindow::update()
{
    RenderWindow::clear(Color(47, 47, 47));

    spriteToRender = memory->spriteRam[spriteIndex];
    if(spriteToRender.isTwoTiles()){
        if(spriteToRender.isAttachedHorizontally()){
            spriteToRender.sprite.setOrigin(16,8);
        }else{
            spriteToRender.sprite.setOrigin(8,16);
        }
    }else{
        spriteToRender.sprite.setOrigin(8,8);
    }
    spriteToRender.sprite.setPosition(127,127);

    spriteToRender.sprite.setScale(Vector2f(scaleFactor,scaleFactor));
    RenderWindow::draw(spriteToRender.sprite);
}

void SpriteRenderWindow::setSpriteIndex(unsigned char spriteIndex)
{
    this->spriteIndex = spriteIndex;
}

unsigned char SpriteRenderWindow::getSpriteIndex()
{
    return spriteIndex;
}

void SpriteRenderWindow::setScaleFactor(unsigned int scaleFactor)
{
    this->scaleFactor = scaleFactor;
}

unsigned int SpriteRenderWindow::getScaleFactor()
{
    return scaleFactor;
}

SpriteRenderWindow::~SpriteRenderWindow()
{

}
