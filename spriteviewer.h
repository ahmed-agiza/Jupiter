#ifndef SPRITEVIEWER_H
#define SPRITEVIEWER_H

#include <QDialog>
#include "memory.h"
#include "spriterenderwindow.h"
namespace Ui {
class SpriteViewer;
}

class SpriteViewer : public QDialog
{
    Q_OBJECT

public:
    explicit SpriteViewer(QWidget *parent, Memory *memory);
    ~SpriteViewer();
    void setViewOnly(bool);
private slots:
    void on_sizeTilesPushButton_clicked();

    void on_attachementOrientationPushButton_clicked();

    void on_renderingEnabledCheckbox_clicked(bool value);

    void on_spriteIndex_valueChanged(int arg1);

    void on_tile0Index_valueChanged(int arg1);

    void on_tile1Index_valueChanged(int arg1);

    void on_tile0FlippingCheckbox_clicked(bool checked);

    void on_tile1FlippingCheckbox_clicked(bool checked);

    void on_tile0FlippingYCheckbox_clicked(bool checked);

    void on_tile1FlippingYCheckbox_clicked(bool checked);


    void on_layerSpinBox_valueChanged(int arg1);

    void on_scaleXPushButton_clicked();

    void on_scaleYPushButton_clicked();

    void on_positionXlineEdit_editingFinished();

    void on_positionYlineEdit_editingFinished();

    void on_tile0RotateRight_PushButton_clicked();

    void on_tile0RotateLeft_PushButton_clicked();

    void on_tile1RotateLeft_PushButton_clicked();

    void on_tile1RotateRight_PushButton_clicked();

    void on_verticalSlider_valueChanged(int);
signals:
    void renderScreen();

private:
    Ui::SpriteViewer *ui;
    Memory *memory;
    SpriteRenderWindow *spriteRenderWindow;

    void updateDisplay();
};

#endif // SPRITEVIEWER_H
