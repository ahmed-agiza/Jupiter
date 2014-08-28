#ifndef TILELOADER_H
#define TILELOADER_H

#include <QDialog>
#include <QMap>
#include <SFML/Graphics.hpp>
#include "memory.h"
namespace Ui {
class TileLoader;
}

class TileLoader : public QDialog
{
    Q_OBJECT

public:
    explicit TileLoader(QWidget *parent, Memory *memory);
    ~TileLoader();
    void setMemory(Memory*);
    sf::Uint32 colorToInt(sf::Color);
private:
    Memory* memory;
    QString filePath;
    QMap< Uint32, Uint8 > paletteSet;
    sf::Uint8 palettePointer;
private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_lineEdit_editingFinished();

private:
    Ui::TileLoader *ui;
};

#endif // TILELOADER_H
