#include "inputmanager.h"
#include "ui_inputmanager.h"
#include <QPixmap>
#include <QBitmap>
#include <QEvent>
#include <QKeyEvent>
#include <QDebug>
#include <SFML/Graphics.hpp>
InputManager::InputManager(QWidget *parent, Memory *memory) :
    QDialog(parent),
    ui(new Ui::InputManager)
{
    this->mem = memory;
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
    installEventFilter(this);

    foreach(QObject *child, children()){
        QWidget *widget = dynamic_cast<QWidget*>(child);
        if (widget){
            widget->setFocusPolicy(Qt::NoFocus);
            widget->clearFocus();
        }
    }

}

bool InputManager::eventFilter(QObject *o, QEvent *e){

    if(e->type() == QEvent::KeyPress){ //Keypressed event.
        QKeyEvent *keyDownEvent = static_cast<QKeyEvent*> (e);
        //Return true/false to accept/ignore the effect of the key press, there is also e->accpet() and e->ignore().
        handleKeyPress(keyDownEvent->key(), keyDownEvent->modifiers());

    }else if (e->type() == QEvent::KeyRelease){ // Keyrelease event.
        QKeyEvent *keyUpEvent = static_cast<QKeyEvent*> (e);
        handleKeyRelease(keyUpEvent->key(), keyUpEvent->modifiers());
    }

    return QDialog::eventFilter(o, e);
}


/*
void InputManager::keyPressEvent(QKeyEvent *e){
    if(e->key() == Qt::Key_Up)
        qDebug() << "Up is pressed";
    else if (e->key() == Qt::Key_Down){
        e->ignore();
        qDebug() << "Ignored key down press";
    }else
        qDebug() << e->text();

}

void InputManager::keyReleaseEvent(QKeyEvent *e){
    if(e->key() == Qt::Key_Up)
        qDebug() << "Up is released";
    else if (e->key() == Qt::Key_Down){
        e->ignore();
        qDebug() << "Ignored key down release";
    }else
        qDebug() << e->text();
}*/

InputManager::~InputManager()
{
    delete ui;
}

void InputManager::handleKeyPress(int key, Qt::KeyboardModifiers modifiers)
{
    if(key == Qt::Key_Up && (modifiers & Qt::ControlModifier)){

        qDebug() << "Up and Ctrl combination was pressed";

    }else
        qDebug() << "Key(" << key << ") key was pressed";

    switch(key){
    case Qt::Key_Up:
        ui->up->setChecked(true);
        break;
    case Qt::Key_Down:
        ui->down->setChecked(true);
        break;
    case Qt::Key_Left:
        ui->left->setChecked(true);
        break;
    case Qt::Key_Right:
        ui->right->setChecked(true);
        break;
    case Qt::Key_Z:
        ui->A->setChecked(true);
        break;
    case Qt::Key_X:
        ui->B->setChecked(true);
        break;
    case Qt::Key_A:
        ui->l->setChecked(true);
        break;
    case Qt::Key_D:
        ui->r->setChecked(true);
        break;
    case Qt::Key_Return:
        ui->start->setChecked(true);
        break;
    case Qt::Key_Backspace:
        ui->select->setChecked(true);
        break;
    }

}

void InputManager::handleKeyRelease(int key, Qt::KeyboardModifiers modifiers)
{
    qDebug() << "Key(" << key << ") key was released";

    switch(key){
    case Qt::Key_Up:
        ui->up->setChecked(false);
        break;
    case Qt::Key_Down:
        ui->down->setChecked(false);
        break;
    case Qt::Key_Left:
        ui->left->setChecked(false);
        break;
    case Qt::Key_Right:
        ui->right->setChecked(false);
        break;
    case Qt::Key_Z:
        ui->A->setChecked(false);
        break;
    case Qt::Key_X:
        ui->B->setChecked(false);
        break;
    case Qt::Key_A:
        ui->l->setChecked(false);
        break;
    case Qt::Key_D:
        ui->r->setChecked(false);
        break;
    case Qt::Key_Return:
        ui->start->setChecked(false);
        break;
    case Qt::Key_Backspace:
        ui->select->setChecked(false);
        break;
    }

}
