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
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{


    ui->setupUi(this);
    setCentralWidget(ui->dockCode);
    installEventFilter(this);


}

bool MainWindow::eventFilter(QObject *, QEvent *e)
{
    if (e->type() == QEvent::Show)
        on_actionNew_triggered();
}



MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_actionNew_triggered()
{



    QMdiSubWindow *newWindow = new QMdiSubWindow(ui->mdiAreaCode);
    newWindow->setObjectName("newW");
    newWindow->setWindowTitle("Untitled");


    QWidget *newWidgets = new QWidget(newWindow);
    newWidgets->setObjectName("NW");
    QHBoxLayout *HL = new QHBoxLayout(newWidgets);
    CodeEditor *newCode = new CodeEditor(newWidgets);
    QTextEdit *linesCount = new QTextEdit(newWidgets);

    newCode->setObjectName("CodeE");

    linesCount->setEnabled(false);
    linesCount->setMinimumSize(10, 10);
    linesCount->setMaximumSize(40, linesCount->maximumSize().height());
    linesCount->setText("0");
    newCode->setCounter(linesCount);

    HL->setObjectName("HL");
    HL->addWidget(linesCount);
    HL->addWidget(newCode);

    newWidgets->setLayout(HL);

    newWindow->setWidget(newWidgets);

    newWindow->setAttribute(Qt::WA_DeleteOnClose, 1);
    ui->mdiAreaCode->addSubWindow(newWindow);
    newWidgets->showMaximized();


}

void MainWindow::printS()
{
    qDebug() << "Test slot";
}

void MainWindow::on_actionAssemble_triggered()
{

   if (ui->mdiAreaCode->currentSubWindow())
   {
        QWidget *W  = ui->mdiAreaCode->currentSubWindow()->findChild <QWidget *> ("NW");
        if (W)
        {
            CodeEditor  *E = W->findChild  <CodeEditor*> ("CodeE");
            if (E)
            {
                qDebug() << E->toPlainText();
                QStringList instrs = E->toPlainText().split("\n");
                Assembler A(&instrs);
            }
            else
                QMessageBox::critical(this, "Error", "Error 1");
        }
        else
            QMessageBox::critical(this, "Error", "Error 2");

   }
   else
        QMessageBox::critical(this, "Error", "Error 3");
}

void MainWindow::on_actionClose_triggered()
{
    ui->mdiAreaCode->closeAllSubWindows();
}
