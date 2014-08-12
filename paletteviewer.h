#ifndef PALETTEVIEWER_H
#define PALETTEVIEWER_H

#include <QDialog>
#include <QMouseEvent>
#include "memory.h"
#include "paletterenderwindow.h"

namespace Ui {
class PaletteViewer;
}

class PaletteViewer : public QDialog
{
    Q_OBJECT

public:
    explicit PaletteViewer(QWidget *parent, Memory *);
    ~PaletteViewer();
public slots:
    void update();
    void mouseMoveEvent(QMouseEvent* event);
private:
    Ui::PaletteViewer *ui;
    Memory* memory;
    PaletteRenderWindow *paletteRenderWindow;

    void changeValue(Vector2i mousePosition);
signals:
    void firstTimeRender();
};

#endif // PALETTEVIEWER_H
