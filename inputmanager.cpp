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
    QPixmap pixmap("C:/stuff/buttons/up.png");
    ui->up->setMask(pixmap.createMaskFromColor(Qt::transparent,Qt::MaskInColor));
    pixmap.load("C:/stuff/buttons/down.png");
    ui->down->setMask(pixmap.createMaskFromColor(Qt::transparent,Qt::MaskInColor));
    pixmap.load("C:/stuff/buttons/left.png");
    ui->left->setMask(pixmap.createMaskFromColor(Qt::transparent,Qt::MaskInColor));
    pixmap.load("C:/stuff/buttons/right.png");
    ui->right->setMask(pixmap.createMaskFromColor(Qt::transparent,Qt::MaskInColor));
    pixmap.load("C:/stuff/buttons/ab2.png");
    ui->A->setMask(pixmap.createMaskFromColor(Qt::transparent,Qt::MaskInColor));
    ui->B->setMask(pixmap.createMaskFromColor(Qt::transparent,Qt::MaskInColor));
    pixmap.load("C:/stuff/buttons/startselectbutton.png");
    ui->start->setMask(pixmap.createMaskFromColor(Qt::transparent,Qt::MaskInColor));
    ui->select->setMask(pixmap.createMaskFromColor(Qt::transparent,Qt::MaskInColor));

    pixmap.load("C:/stuff/buttons/l.png");
    ui->l->setMask(pixmap.createMaskFromColor(Qt::transparent,Qt::MaskInColor));
    pixmap.load("C:/stuff/buttons/r.png");
    ui->r->setMask(pixmap.createMaskFromColor(Qt::transparent,Qt::MaskInColor));

}

InputManager::~InputManager()
{
    delete ui;
}
