#ifndef TILESETVIEWER_H
#define TILESETVIEWER_H

#include <QDialog>
#include "memory.h"
#include "tilerenderwindow.h"

namespace Ui {
class TileSetViewer;
}

class TileSetViewer : public QDialog
{
    Q_OBJECT

public:
    explicit TileSetViewer(QWidget *parent, Memory *);
    ~TileSetViewer();

private:
    Ui::TileSetViewer *ui;
    Memory* memory;
    TileRenderWindow *tileRenderWindow;
};

#endif // TILESETVIEWER_H
