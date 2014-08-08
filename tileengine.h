#ifndef TILEENGINE_H
#define TILEENGINE_H
#include <SFML/Graphics.hpp>
#include "memory.h"
#include "qrenderwindow.h"

class TileEngine: public QRenderWindow
{
private:
    Memory* memory;
    Clock clk;
    Time time;
    int framesPerSecond;

    Vector2u screenSize;
    unsigned int horizontalScroll, verticalScroll;
public:
    TileEngine(QWidget*, const QPoint&, const QSize&);
    ~TileEngine();
    void initialize();
    void update();
    void renderFrame();
signals:
    void renderNow();
public slots:
    void updateNow();
};

#endif // TILEENGINE_H
