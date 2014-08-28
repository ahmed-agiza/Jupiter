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
#include "memory.h"
#include "InstructionFuncs.h"
#include "memorymodel.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow){


    ui->setupUi(this);
    setCentralWidget(ui->dockCode);
    installEventFilter(this);

    memory = new Memory(this);

    memoryLoading = NULL;

    for (int i = 0; i < 32; i++){
        mainProcessorRegisters.append(0);
    }
    mainProcessorRegisters[28] = 0x10008000;
    mainProcessorRegisters[29] = 0x7FFFFFFC;

    RegistersModel *regModel = new RegistersModel(&mainProcessorRegisters, this);
    ui->tableMainRegisters->setModel(regModel);

    assem = NULL;
    assemblerInitialized = false;
    this->setMouseTracking(true);

    ui->actionTileset_viewer->setEnabled(false);
    ui->actionBitmap_Display->setEnabled(false);
    ui->actionPalette_Viewer->setEnabled(false);
    ui->actionReload_Tiles_Memory->setEnabled(false);
    ui->actionTile_loader->setEnabled(false);

    /*Memory *testMemory = new Memory(this);
    int location = testMemory->dataSegmentBaseAddress;
    testMemory->storeWord(location, 15614);
    testMemory->storeWord(location + 4, 1179010630);
    testMemory->storeByte(location + 5, '5');
    testMemory->storeByte(location + 6, '6');
    testMemory->storeByte(location + 7, '7');
    testMemory->storeByte(location + 8, '8');*/




    MemoryModel *textModel = new MemoryModel(memory, this, TextSegment, ui->textAddressMode, ui->textMemoryMode, ui->textMemoryBase);
    MemoryModel *dataModel = new MemoryModel(memory, this, DataSegment, ui->dataAddressMode, ui->dataMemoryMode, ui->dataMemoryBase);
    MemoryModel *stackModel = new MemoryModel(memory, this, StackSegment, ui->stackAddressMode, ui->stackMemoryMode, ui->stackMemoryBase);
    MemoryModel *heapModel = new MemoryModel(memory, this, HeapSegment, ui->heapAddressMode, ui->heapMemoryMode, ui->heapMemoryBase);

    ui->textTable->setModel(textModel);
    ui->dataTable->setModel(dataModel);
    ui->stackTable->setModel(stackModel);
    ui->heapTable->setModel(heapModel);
    /*for(int i = location; i < location + 32; i+=4){
        qDebug() << "i: " << i << ":  " << (int) testMemory->loadWordU(i) << "\n";
    }*/

}


bool MainWindow::eventFilter(QObject *, QEvent *e){
    if (e->type() == QEvent::Show)
        on_actionNew_triggered();
    return false;
}



MainWindow::~MainWindow(){
    //delete memory;
    if(assemblerInitialized)
        delete assem;
    delete ui;
}

void MainWindow::on_actionSimulate_triggered(){
    engine = new TileEngine(0, QPoint(0,0), QSize(512,384), memory);
    memory->setTileEngine(engine);
    engine->show();
    if (assemblerInitialized){
        assem->simulate();
        mainProcessorRegisters = *assem->registers;
    }
}

void MainWindow::on_actionNew_triggered(){

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

void MainWindow::printS(){

}

void MainWindow::on_actionAssemble_triggered(){
    if (ui->mdiAreaCode->currentSubWindow())
    {
        QWidget *W  = ui->mdiAreaCode->currentSubWindow()->findChild <QWidget *> ("NW");
        if (W){
            CodeEditor  *E = W->findChild  <CodeEditor*> ("CodeE");
            if (E){
                qDebug() << E->toPlainText();
                QStringList instrs = E->toPlainText().split("\n");
                if(assemblerInitialized)
                    delete assem;
                assem = new Assembler(&instrs, memory, &mainProcessorRegisters);
                assemblerInitialized = true;
            }else
                QMessageBox::critical(this, "Error", "Error 1");
        }else
            QMessageBox::critical(this, "Error", "Error 2");

    }else
        QMessageBox::critical(this, "Error", "Error 3");
}

void MainWindow::on_actionClose_triggered(){
    ui->mdiAreaCode->closeAllSubWindows();
}

void MainWindow::on_actionTileset_viewer_triggered(){
    tileSetViewer = new TileSetViewer(this, memory);
    tileSetViewer->show();
}

void MainWindow::on_actionPalette_Viewer_triggered(){
    paletteViewer = new PaletteViewer(this, memory);
    paletteViewer->show();
}

void MainWindow::on_actionTile_loader_triggered(){
    tileLoader = new TileLoader(this, memory);
    tileLoader->show();
}

void MainWindow::on_actionEnable_Graphics_Engine_triggered()
{
    if(ui->actionEnable_Graphics_Engine->isChecked()){
        if(memoryLoading == NULL || !memoryLoading->isInit()){
            if (memoryLoading == NULL)
                memoryLoading = new MemoryLoading(0, this->memory);
            memoryLoading->setModal(true);
            memoryLoading->show();
        }
        ui->actionTileset_viewer->setEnabled(true);
        ui->actionBitmap_Display->setEnabled(true);
        ui->actionPalette_Viewer->setEnabled(true);
        ui->actionReload_Tiles_Memory->setEnabled(true);
        ui->actionTile_loader->setEnabled(true);
    }else{
        ui->actionTileset_viewer->setEnabled(false);
        ui->actionBitmap_Display->setEnabled(false);
        ui->actionPalette_Viewer->setEnabled(false);
        ui->actionReload_Tiles_Memory->setEnabled(false);
        ui->actionTile_loader->setEnabled(false);
    }
}

void MainWindow::on_actionReload_Tiles_Memory_triggered()
{

    if (memoryLoading != NULL){
        if (memoryLoading->isVisible()){
            memoryLoading->show();
            return;
        }
        delete memoryLoading;
    }

    memoryLoading = new MemoryLoading(0, this->memory);
    memoryLoading->setModal(true);
    memoryLoading->show();
}
