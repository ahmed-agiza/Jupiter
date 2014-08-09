#include "qrenderwindow.h"
#ifdef Q_WS_X11
#include <Qt/qx11info_x11.h>
#include <X11/Xlib.h>
#endif
#include <iostream>

QRenderWindow::QRenderWindow(QWidget* Parent, const QPoint& Position, const QSize& Size, unsigned int FrameTime) : QWidget(Parent),
    myInitialized (false)
{
    // Setup some states to allow direct rendering into the widget
    setAttribute(Qt::WA_PaintOnScreen);
    setAttribute(Qt::WA_OpaquePaintEvent);
    setAttribute(Qt::WA_NoSystemBackground);
    // Set strong focus to enable keyboard events to be received
    setFocusPolicy(Qt::StrongFocus);
    // Setup the widget geometry
    move(Position);
    resize(Size);
    // Setup the timer
    myTimer.setInterval(FrameTime);
}
QRenderWindow::~QRenderWindow() {}
void QRenderWindow::showEvent(QShowEvent*)
{
    if (!myInitialized)
    {
        // Under X11, we need to flush the commands sent to the server to ensure that
        // SFML will get an updated view of the windows
#ifdef Q_WS_X11
        XFlush(QX11Info::display());
#endif
        // Create the SFML window with the widget handle
        sf::RenderWindow::create(reinterpret_cast<sf::WindowHandle>(winId()));
        // Let the derived class do its specific stuff
        initialize();
        connect(&myTimer, SIGNAL(timeout()), this, SLOT(repaint()));
        // Setup the timer to trigger a refresh at specified framerate
        myTimer.start();
        myInitialized = true;
    }
}
QPaintEngine* QRenderWindow::paintEngine() const
{
    return 0;
}
void QRenderWindow::paintEvent(QPaintEvent*)
{
    update();
    sf::RenderWindow::display();
}
void QRenderWindow::initialize() {}
void QRenderWindow::update() {}
