#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->subwindowMain->showMaximized();


}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_textCodeEditor_textChanged()
{
    ui->codeLineNumber->clear();
    ui->codeLineNumber->setAlignment(Qt::AlignCenter);
    int numLines = ui->textCodeEditor->toPlainText().count("\n");
    qDebug() << numLines;
    for (int i = 0; i <= numLines; i++)
        ui->codeLineNumber->append(QString::number(i));
}
