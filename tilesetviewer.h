#ifndef TILESETVIEWER_H
#define TILESETVIEWER_H

#include <QDialog>
#include <QFileDialog>
#include <QMessageBox>
#include "memory.h"
#include "tilerenderwindow.h"
#include <SFML/Graphics.hpp>

namespace Ui {
class TileSetViewer;
}

class TileSetViewer : public QDialog
{
    Q_OBJECT

public:
    explicit TileSetViewer(QWidget *parent, Memory *);
    ~TileSetViewer();

    sf::Uint32 colorToInt(sf::Color);

private slots:
    void on_pushButton_clicked();

    void on_horizontalSlider_valueChanged(int value);

    void on_verticalSlider_valueChanged(int value);

    void on_browsePushButton_clicked();

    void on_loadPushButton_clicked();

    void on_lineEdit_editingFinished();

    void on_savePushButton_clicked();

private:
    Ui::TileSetViewer *ui;
    Memory* memory;
    TileRenderWindow *tileRenderWindow;
    QString filePath;
    QMap< Uint32, Uint8 > paletteSet;
    sf::Uint8 palettePointer;

signals:
    void renderScreen();
};

#endif // TILESETVIEWER_H
