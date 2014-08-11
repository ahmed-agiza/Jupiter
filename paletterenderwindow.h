#ifndef PALETTERENDERWINDOW_H
#define PALETTERENDERWINDOW_H
#include "qrenderwindow.h"
#include "memory.h"

class PaletteRenderWindow: public QRenderWindow
{
public:
    PaletteRenderWindow(QWidget *parent, Memory *);
    ~PaletteRenderWindow();
    void initialize();
    void update();
    void setPaletteIndex(unsigned char);
    unsigned char getPaletteIndex();
private:
    Memory *memory;
    unsigned char paletteIndex;
    sf::Color color;
signals:
    void renderScreen();
};

#endif // PALETTERENDERWINDOW_H
