#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QStringListModel>
#include <QLineEdit>
#include <QTextEdit>
#include <QMdiSubWindow>
#include <QMdiArea>
#include <QPair>
#include "codeeditor.h"
#include <QHBoxLayout>
#include <QPalette>
#include "instruction.h"
#include <QVector>
#include <QMessageBox>
#include <iostream>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QDomDocument>
#include <QDomElement>
#include <QDomText>

#include "memory.h"
#include "InstructionFuncs.h"




MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow){


    ui->setupUi(this);
    setCentralWidget(ui->dockCode);
    installEventFilter(this);

    memory = new Memory(this);

    memoryLoading = NULL;
    ui->tableMemory->setCurrentIndex(1);

    for (int i = 0; i < 32; i++){
        mainProcessorRegisters.append(0);
    }
    mainProcessorRegisters[28] = 0x10008000;
    mainProcessorRegisters[29] = 0x7FFFFFFC;

    regModel = new RegistersModel(&mainProcessorRegisters, this, ui->registersNaming, ui->registersBase);
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




    textModel = new MemoryModel(memory, this, TextSegment, ui->textAddressMode, ui->textMemoryMode, ui->textMemoryBase);
    dataModel = new MemoryModel(memory, this, DataSegment, ui->dataAddressMode, ui->dataMemoryMode, ui->dataMemoryBase);
    stackModel = new MemoryModel(memory, this, StackSegment, ui->stackAddressMode, ui->stackMemoryMode, ui->stackMemoryBase);
    heapModel = new MemoryModel(memory, this, HeapSegment, ui->heapAddressMode, ui->heapMemoryMode, ui->heapMemoryBase);

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
    if (engine)
        delete engine;
    delete ui;
}

void MainWindow::on_actionSimulate_triggered(){
    // qDebug() << "Simulating..";
    engine = new TileEngine(0, QPoint(0,0), QSize(512,384), memory);
    memory->setTileEngine(engine);
    if(ui->actionEnable_Graphics_Engine->isChecked()){
        engine->show();
    }

    if (assemblerInitialized){
        assem->simulate();
        mainProcessorRegisters = *assem->registers;


        regModel = new RegistersModel(&mainProcessorRegisters, this, ui->registersNaming, ui->registersBase);
        ui->tableMainRegisters->setModel(regModel);

        textModel = new MemoryModel(memory, this, TextSegment, ui->textAddressMode, ui->textMemoryMode, ui->textMemoryBase);
        dataModel = new MemoryModel(memory, this, DataSegment, ui->dataAddressMode, ui->dataMemoryMode, ui->dataMemoryBase);
        stackModel = new MemoryModel(memory, this, StackSegment, ui->stackAddressMode, ui->stackMemoryMode, ui->stackMemoryBase);
        heapModel = new MemoryModel(memory, this, HeapSegment, ui->heapAddressMode, ui->heapMemoryMode, ui->heapMemoryBase);

        ui->textTable->setModel(textModel);
        ui->dataTable->setModel(dataModel);
        ui->stackTable->setModel(stackModel);
        ui->heapTable->setModel(heapModel);
        // qDebug() << "Simulated.";
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
    // qDebug() << "Assembling..";
    if (ui->mdiAreaCode->currentSubWindow())
    {
        QWidget *W  = ui->mdiAreaCode->currentSubWindow()->findChild <QWidget *> ("NW");
        if (W){
            CodeEditor  *E = W->findChild  <CodeEditor*> ("CodeE");
            if (E){
                //qDebug() << E->toPlainText();
                QStringList instrs = E->toPlainText().split("\n");
                if(assemblerInitialized){
                    delete assem;
                    if (memory){
                        Memory *tempMemory = memory;
                        memory = new Memory(this);
                        delete tempMemory;
                    }

                }
                assem = new Assembler(&instrs, memory, &mainProcessorRegisters);
                assemblerInitialized = true;
            }else
                QMessageBox::critical(this, "Error", "Error 1");
        }else
            QMessageBox::critical(this, "Error", "Error 2");

    }else
        QMessageBox::critical(this, "Error", "Error 3");
    //qDebug() << "Assembled.";
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

void MainWindow::on_actionAssemble_and_Simulate_triggered()
{
    ui->actionAssemble->trigger();
    ui->actionSimulate->trigger();
}

void MainWindow::on_actionOpen_Project_triggered()
{
    projectPath = ":/testProject/testProject.mpro";
    QFile file(projectPath);
    QString data;
    if (file.open(QIODevice::ReadOnly)){
        file.close();
        parseProjectXML(file);
    }else{
        QMessageBox::information(0, "error", file.errorString());
        qDebug() << "Failed to open!";
    }
    qDebug() << data;

}

void MainWindow::parseProjectXML(QFile &data){
    /*if (data.open(QIODevice::ReadOnly)){
        QXmlStreamReader xml(&data);
        while(!xml.atEnd()){
            if(xml.isStartElement())
            {
                // Read the tag name.
                QString tagName(xml.name().toString());
                // Check in settings map, whether there's already an entry. If not, insert.
                QString tagValue(xml.readElementText());
                qDebug() << tagName << ":  " << tagValue;
                projectMap[tagName] = tagValue;
            }
            xml.readNext();
        }
    }*/
    if(!data.open( QIODevice::ReadOnly | QIODevice::Text ) ){
        qDebug( "Failed to open!" );
    }else{
        QDomDocument domDocument;
        if(!domDocument.setContent(&data)){
            qDebug( "Cannot set content" );
            return;
        }

        data.close();

        QDomElement domElement = domDocument.documentElement();
        QDomNode child = domElement.firstChild();
        while(!child.isNull()) {
            QDomElement e = child.toElement();
            if (e.tagName().trimmed() == "ProjectTitle"){
                projectTitle = e.toElement().text().trimmed();
                qDebug() << "Title: " << projectTitle;
            }else if (e.tagName().trimmed() == "TextSegment"){
                QDomNodeList textL = e.childNodes();
                projectTextFiles.clear();;
                for (int i = 0; i < textL.size(); i++){
                    QString tempTag = textL.at(i).toElement().tagName().trimmed();
                    QString tempTagValue = textL.at(i).toElement().text().trimmed();

                    if (tempTag == "MainFile"){
                        qDebug() << "Main: " << tempTagValue;
                        projectMainFile = tempTagValue;
                        projectTextFiles.append(tempTagValue);
                    }else if (tempTag == "File"){
                        projectTextFiles.append(tempTagValue);
                    }
                    if (i < projectTextFiles.size())
                        qDebug() << "Text" << i << ": " << projectTextFiles.at(i);
                }
            }else if (e.tagName().trimmed() == "DataSegment"){
                projectDataFile = e.firstChild().toElement().text().trimmed();
                qDebug() << "Data: " << projectDataFile;
            }else if (e.tagName().trimmed() == "Configure"){
                QDomNodeList confL = e.childNodes();
                projectConf.clear();
                for (int i = 0; i < confL.size(); i++){
                    projectConf[confL.at(i).toElement().tagName().trimmed()] = confL.at(i).toElement().text().trimmed();
                    qDebug() << confL.at(i).toElement().tagName().trimmed() << ": " << projectConf[confL.at(i).toElement().tagName().trimmed()];
                }
            }
            child = child.nextSibling();

        }

    }
    /* QString projectPath;
    QString projectTitle;
    QString mainFile;
    QStringList projectTextFiles;
    QString projectDataFile;
    QMap<QString, QString> projectConf;*/




}
