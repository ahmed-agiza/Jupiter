#include "inputmanager.h"
#include "ui_inputmanager.h"

InputManager::InputManager(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InputManager)
{
    ui->setupUi(this);
}

InputManager::~InputManager()
{
    delete ui;
}