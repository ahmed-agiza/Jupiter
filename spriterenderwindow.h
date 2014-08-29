#ifndef SPRITERENDERWINDOW_H
#define SPRITERENDERWINDOW_H

#include "qrenderwindow.h"
#include "memory.h"

class SpriteRenderWindow : public QRenderWindow
{
public:
    SpriteRenderWindow(QWidget *parent, Memory *);
    ~SpriteRenderWindow();
    void initialize();
    void update();
    void setSpriteIndex(unsigned char);
    unsigned char getSpriteIndex();
    void setScaleFactor(unsigned int scaleFactor);
    unsigned int getScaleFactor();
private:
    GameSprite spriteToRender;
    unsigned char spriteIndex;
    Memory *memory;
    unsigned int scaleFactor;
signals:
    void renderScreen();

};

#endif // SPRITERENDERWINDOW_H
