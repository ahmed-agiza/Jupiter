#ifndef PALETTEVIEWER_H
#define PALETTEVIEWER_H

#include <QDialog>
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

private slots:
    void on_horizontalSlider_valueChanged(int value);

private:
    Ui::PaletteViewer *ui;
    Memory* memory;
    PaletteRenderWindow *paletteRenderWindow;
    void changeValue(int);

signals:
    void renderScreen();
};

#endif // PALETTEVIEWER_H
