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

    joystickNumber = 0;

    buttons.resize(10);

    buttons[ DOWN_KEY_INDEX] =  ui->down;
    buttons[ LEFT_KEY_INDEX] =  ui->left;
    buttons[ RIGHT_KEY_INDEX] =  ui->right;
    buttons[ UP_KEY_INDEX] =  ui->up;
    buttons[ A_KEY_INDEX] =  ui->A;
    buttons[ B_KEY_INDEX] =  ui->B;
    buttons[ R_KEY_INDEX] =  ui->r;
    buttons[ L_KEY_INDEX] =  ui->l;
    buttons[ START_KEY_INDEX] =  ui->start;
    buttons[ SELECT_KEY_INDEX] =  ui->select;


    connect(this,SIGNAL(buttonPressed(int,int,bool)),mem, SLOT(updateKey(int, int, bool)));
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


    for(int i=0; i<8; i++){
        ui->tableWidget->setColumnWidth(i,30);
    }

    for(int i=0; i<2; i++){
        for(int j=0; j<8; j++){
            tablePointers[i][j] = new QTableWidgetItem("0");
            tablePointers[i][j]->setTextAlignment(Qt::AlignCenter);
            ui->tableWidget->setItem(i,j,tablePointers[i][j]);
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
    for(int i=0; i<2; i++){
        for(int j=0; j<8; j++){
            delete tablePointers[i][j];
        }
    }
}

void InputManager::handleKeyPress(int key, Qt::KeyboardModifiers modifiers)
{
    if(keyman.contains(key)){
        emit buttonPressed(keyman.getJoystickKeyCode(key), 0, true);
        refreshTable();
        //buttons[keyman.getJoystickKeyCode(key)]->setChecked(true);
    }
/*
    switch(key){
    case Qt::Key_Up:
        emit buttonPressed(UP_KEY_INDEX,0, true);
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
*/

}

void InputManager::refreshTable()
{
    unsigned int address = mem->inputMemoryBaseAddress + (joystickNumber<<1);
    Uint8 returnedbuttons0 = mem->loadByte(address);
    Uint8 returnedbuttons1 = mem->loadByte(address+1);

    for(int i=0; i<8; i++){
        //ui->tableWidget->setItem(0,7-i, &items[((returnedbuttons0>>i)&1)]);
        //ui->tableWidget->setItem(1,7-i, &items[((returnedbuttons1>>i)&1)]);
        tablePointers[0][7-i]->setText(QString::number(((returnedbuttons0>>i)&1)));
        tablePointers[1][7-i]->setText(QString::number(((returnedbuttons1>>i)&1)));
        buttons[i]->setChecked((returnedbuttons0>>i)&1);
    }
    buttons[8]->setChecked((returnedbuttons1)&1);
    buttons[9]->setChecked((returnedbuttons1>>1)&1);
}

void InputManager::handleKeyRelease(int key, Qt::KeyboardModifiers modifiers)
{
    if(keyman.contains(key)){
        emit buttonPressed(keyman.getJoystickKeyCode(key), 0, false);
        refreshTable();
        //buttons[keyman.getJoystickKeyCode(key)]->setChecked(false);
    }
    /*switch(key){
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
*/
}

void InputManager::on_pushButton_pressed()
{
    if(ui->pushButton->text() == "Joypad 1"){
        ui->pushButton->setText("Joypad 2");
        joystickNumber = 1;
    }else{
        ui->pushButton->setText("Joypad 1");
        joystickNumber = 0;
    }
}
