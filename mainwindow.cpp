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
#include "instruction.h"
#include <QVector>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{


    ui->setupUi(this);
    on_actionNew_triggered();
    this->setCentralWidget(ui->dockCode);

    QVector<__int32> regs;
    for (int i = 0; i < 40; i++) regs.push_back(i);
    instruction("add" , &regs, 0, 2, 3, 4, 0, 0, IFormat);

    qDebug() << (-4 >> 1);



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
    linesCount->setEnabled(false);
    linesCount->setMinimumSize(10, 10);
    linesCount->setMaximumSize(40, linesCount->maximumSize().height());
    linesCount->setText("0");
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
