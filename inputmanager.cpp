#include "inputmanager.h"
#include "ui_inputmanager.h"
#include <QPixmap>
#include <QBitmap>

InputManager::InputManager(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InputManager)
{
    ui->setupUi(this);
    this->setWindowTitle("Input Manager");
    QPixmap pixmap;
    pixmap.load(":/joypad/icons/joypad/up.png");
    ui->up->setMask(pixmap.createMaskFromColor(Qt::transparent,Qt::MaskInColor));
    pixmap.load(":/joypad/icons/joypad/down.png");
    ui->down->setMask(pixmap.createMaskFromColor(Qt::transparent,Qt::MaskInColor));
    pixmap.load(":/joypad/icons/joypad/left.png");
    ui->left->setMask(pixmap.createMaskFromColor(Qt::transparent,Qt::MaskInColor));
    pixmap.load(":/joypad/icons/joypad/right.png");
    ui->right->setMask(pixmap.createMaskFromColor(Qt::transparent,Qt::MaskInColor));
    pixmap.load(":/joypad/icons/joypad/ab2.png");
    ui->A->setMask(pixmap.createMaskFromColor(Qt::transparent,Qt::MaskInColor));
    ui->B->setMask(pixmap.createMaskFromColor(Qt::transparent,Qt::MaskInColor));
    pixmap.load(":/joypad/icons/joypad/startselectbutton.png");
    ui->start->setMask(pixmap.createMaskFromColor(Qt::transparent,Qt::MaskInColor));
    ui->select->setMask(pixmap.createMaskFromColor(Qt::transparent,Qt::MaskInColor));

    pixmap.load(":/joypad/icons/joypad/l.png");
    ui->l->setMask(pixmap.createMaskFromColor(Qt::transparent,Qt::MaskInColor));
    pixmap.load(":/joypad/icons/joypad/r.png");
    ui->r->setMask(pixmap.createMaskFromColor(Qt::transparent,Qt::MaskInColor));

}

InputManager::~InputManager()
{
    delete ui;
}
