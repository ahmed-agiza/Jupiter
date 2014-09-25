#ifndef QRENDERWINDOW_H
#define QRENDERWINDOW_H

#include <QWidget>
#include <SFML/Graphics.hpp>
#include "memory.h"
#include <QTimer>

class QRenderWindow : public QWidget, public sf::RenderWindow
{
    Q_OBJECT
public:
    explicit QRenderWindow(QWidget *parent, const QPoint& Position, const QSize& Size, unsigned int FrameTime = 0);
    virtual void showEvent(QShowEvent*);
    virtual QPaintEngine* paintEngine() const;
    virtual void paintEvent(QPaintEvent*);
    virtual ~QRenderWindow();
    virtual void initialize();
    virtual void update();
private:
    QTimer myTimer;
    bool myInitialized;
signals:
    void setSpritePosition(int, int, Vector2f);
    void setSpriteOrigin(int, int, Vector2f);

};

#endif // QRENDERWINDOW_H
