#ifndef TILERENDERWINDOW_H
#define TILERENDERWINDOW_H

#include "qrenderwindow.h"
#include "memory.h"
class TileRenderWindow : public QRenderWindow
{
public:
    TileRenderWindow(QWidget *parent, Memory *);
    ~TileRenderWindow();
    void initialize();
    void update();
    void setTileSetToRender(bool);
    void setTileIndex(unsigned char);
    bool getTileSetToRender();
    unsigned char getTileIndex();
private:
    sf::Sprite tileToRender;
    bool tileSetToRender;
    unsigned char tileIndex;
    Memory *memory;
signals:

public slots:

};

#endif // TILERENDERWINDOW_H
