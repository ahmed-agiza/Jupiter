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
#include <iostream>
#include "registersmodel.h"

#include "InstructionFuncs.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{


    ui->setupUi(this);
    setCentralWidget(ui->dockCode);
    installEventFilter(this);

    memory = new Memory;
    memoryLoading = new MemoryLoading(0, this->memory);
    memoryLoading->show();

    for (int i = 0; i < 32; i++){
        mainProcessorRegisters.append(0);
    }
    mainProcessorRegisters[28] = 0x10008000;
    mainProcessorRegisters[29] = 0x7FFFEFFC;

    RegistersModel *regModel = new RegistersModel(&mainProcessorRegisters, this);
    ui->tableMainRegisters->setModel(regModel);

    assem = NULL;
    assemblerInitialized = false;
    this->setMouseTracking(true);
}

bool MainWindow::eventFilter(QObject *, QEvent *e)
{
    if (e->type() == QEvent::Show)
        on_actionNew_triggered();
    return false;
}



MainWindow::~MainWindow()
{
    delete memory;
    if(assemblerInitialized)
        delete assem;
    delete ui;
}

void MainWindow::on_actionSimulate_triggered()
{
    if (assemblerInitialized){
        assem->simulate();
        mainProcessorRegisters = *assem->registers;
    }

    engine = new TileEngine(0, QPoint(0,0), QSize(512,384), memory);
    memory->setTileEngine(engine);
    engine->show();
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
                if(assemblerInitialized)
                    delete assem;
                assem = new Assembler(&instrs, memory, &mainProcessorRegisters);
                assemblerInitialized = true;
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

void MainWindow::on_actionTileset_viewer_triggered()
{
    tileSetViewer = new TileSetViewer(this, memory);
    tileSetViewer->show();
}

void MainWindow::on_actionPalette_Viewer_triggered()
{
    paletteViewer = new PaletteViewer(this, memory);
    paletteViewer->show();
}
