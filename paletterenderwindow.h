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
private:
    Memory *memory;
    QVector< QVector<sf::Sprite> > screen;
signals:
    void renderScreen();
};

#endif // PALETTERENDERWINDOW_H
