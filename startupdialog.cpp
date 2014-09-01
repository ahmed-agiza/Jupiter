#include "startupdialog.h"
#include "ui_startupdialog.h"
#include <QApplication>
#include <QDesktopWidget>

#include <QPushButton>

StartupDialog::StartupDialog(MainWindow *parent) :
    QDialog(parent),
    ui(new Ui::StartupDialog)
{
    ui->setupUi(this);
    this->setModal(true);
    this->setFixedSize(this->width(), this->height());
    p = parent;

    QRect screenGeo = QApplication::desktop()->screenGeometry(0);
    QPoint centerPoint = screenGeo.center();
    centerPoint.setX(centerPoint.x() - this->width()/2);
    centerPoint.setY(centerPoint.y() - this->height()/2);
    move(centerPoint);
}

StartupDialog::~StartupDialog()
{
    delete ui;
}

void StartupDialog::on_btnNewProject_clicked()
{
    hide();
}



void StartupDialog::on_btnOpenProject_clicked()
{
    hide();
    p->openProjectAction();
}
