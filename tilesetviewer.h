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

private slots:
    void on_pushButton_clicked();

    void on_horizontalSlider_valueChanged(int value);

private:
    Ui::TileSetViewer *ui;
    Memory* memory;
    TileRenderWindow *tileRenderWindow;

signals:
    void renderScreen();
};

#endif // TILESETVIEWER_H
