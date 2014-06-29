#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QStringListModel>
#include <QLineEdit>
#include <QTextEdit>
#include <QMdiSubWindow>
#include <QMdiArea>
#include "codeeditor.h"
#include <QHBoxLayout>
#include <QPalette>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{


    ui->setupUi(this);
    on_actionNew_triggered();
    this->setCentralWidget(ui->dockCode);



}

MainWindow::~MainWindow()
{
    delete ui;
}






void MainWindow::on_actionNew_triggered()
{
    QWidget *newWidgets = new QWidget(this);

    CodeEditor *newCode = new CodeEditor(this);
    QTextEdit *linesCount = new QTextEdit(this);
    //linesCount->setEnabled(false);
    linesCount->setMinimumSize(10, 10);
    linesCount->setMaximumSize(40, linesCount->maximumSize().height());
    linesCount->setText("0");
    QPalette pal =  linesCount->palette();
    pal.setColor(QPalette::Active, QPalette::Active, Qt::blue);
    linesCount->setPalette(pal);
    newCode->setCounter(linesCount);

    QHBoxLayout *HL = new QHBoxLayout(newWidgets);

    HL->addWidget(linesCount);
    HL->addWidget(newCode);

    newWidgets->setLayout(HL);

    QMdiSubWindow *newWindow = new QMdiSubWindow(ui->mdiAreaCode);
    newWindow->setWindowTitle("Untitled");
    newWindow->setWidget(newWidgets);
    ui->mdiAreaCode->addSubWindow(newWindow);
    newWidgets->showMaximized();

}
