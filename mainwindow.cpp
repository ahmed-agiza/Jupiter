#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QStringListModel>
#include <QLineEdit>
#include <QTextEdit>
#include <QMdiSubWindow>
#include <QMdiArea>
#include <QPair>
#include <QHBoxLayout>
#include <QPalette>
#include <QVector>
#include <QMessageBox>
#include <QDomDocument>
#include <QDomElement>
#include <QDomText>
#include <QSignalMapper>
#include <QFileDialog>
#include <QTreeWidgetItemIterator>
#include <QInputDialog>
#include <QStatusBar>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QPointer>

#include "projectcreator.h"
#include "codeeditor.h"
#include "instruction.h"
#include "fileloader.h"
#include "explorertreeitem.h"
#include "memory.h"
#include "InstructionFuncs.h"
#include "startupdialog.h"
#include "deletefiledialog.h"
#include "dealyedsimulationdialog.h"

QString MainWindow::projectPath;
QString MainWindow::projectFileName;
QString MainWindow::projectTitle;
QString MainWindow::projectMainFile;
QStringList MainWindow::projectTextFiles;
QString MainWindow::projectDataFile;
QMap<QString, QString> MainWindow::projectConf;

int minDataTableWidth0 = 0;
int minDataTableWidth1 = 0;
int minTextTableWidth0 = 0;
int minTextTableWidth1 = 0;
int minHeapTableWidth0 = 0;
int minHeapTableWidth1 = 0;
int minStackTableWidth0 = 0;
int minStackTableWidth1 = 0;
int minRegsTableWidth0 = 0;
int minRegsTableWidth1 = 0;

QString tempProjectTitle;
QString tempProjectMainFile;
QStringList tempProjectTextFiles;
QString tempProjectDataFile;
QMap<QString, QString> tempProjectConf;
QString tempProjectPath;


QMap<int, int> tempMap;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);
    setCentralWidget(ui->dockCode);
    installEventFilter(this);
    setMouseTracking(true);
    setAcceptDrops(true);

    addDefaultFont();

    ui->actionTileset_viewer->setEnabled(false);
    ui->actionPalette_Viewer->setEnabled(false);
    ui->actionReload_Tiles_Memory->setEnabled(false);
    ui->actionTile_loader->setEnabled(false);
    ui->actionSprite_Editor->setEnabled(false);

    codeArea = new CodeMDIArea(ui->dockCode);
    ui->mdiLayout->addWidget(codeArea);

    console = new IOConsole(ui->tabConsole);
    console->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->consoleHLayout->addWidget(console);
    QObject::connect(console, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(consoleMenuRequested(QPoint)), Qt::UniqueConnection);

    treeWidget = ui->treeFiles;
    treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    recDialog = new ReconfigureProjectDialog(this);
    recDialog->hide();

    memory = new Memory(this);

    timer = new QTimer(this);
    timer->setSingleShot(false);
    timer->setInterval(300);
    timer->stop();

    memoryLoading = NULL;

    engine = NULL;
    simulationBar = NULL;

    assemblerInitialized = false;
    assembling = false;
    simulating = false;
    simulateAfterAssembling = false;
    simulationPaused = true;
    simulationSpeed = 0;

    initRegs();
    initMemoryModels(false);

    assem = new Assembler(memory, &mainProcessorRegisters, this);

    engine = new TileEngine(0, QPoint(0,0), QSize(512,384), memory, &mainProcessorRegisters);
    engine->hide();
    memory->setTileEngine(engine);
    //memory->setParent(assem);


    assem->moveToThread(&simulationThread);
    //memory->moveToThread(&simulationThread);


    refreshActions();
    refreshEditActions();
    connectActions();

    setupColumnsResize();


    ui->toolBar->setAccessibleName("Toolbar");


    ui->tabsProject->setCurrentIndex(0);
    ui->tableMemory->setCurrentIndex(1);

    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(simulationProgress()), Qt::UniqueConnection);
    thread()->setPriority(QThread::HighestPriority);
}

void MainWindow::dragEnterEvent(QDragEnterEvent *e){
    if(e->mimeData()->hasUrls())
        if(e->mimeData()->urls().size() == 1){
            if (projectFile.isOpen() || QFileInfo(e->mimeData()->urls().at(0).toLocalFile()).suffix() == "mpro")
                e->acceptProposedAction();
        }

}

void MainWindow::assembleAction(int speed, bool stepped = false){

    if(MainWindow::getProjectMainFile() == ""){
        QMessageBox::critical(this, "Error", "Cannot find main text file");
        return;
    }

    if (!codeArea->isAllSaved()){
        if (QMessageBox::information(this, "Save", "Do you want to save modified file(s) before assembling?", QMessageBox::No, QMessageBox::Yes) == QMessageBox::Yes){
            foreach (QMdiSubWindow *window, codeArea->subWindowList()) {
                CodeEditorWindow* currentWindow = dynamic_cast<CodeEditorWindow *>(window);
                if (currentWindow)
                    currentWindow->saveFile();
            }
        }
    }
    simulationSpeed = speed;

    QStringList textInstrs;
    QStringList dataInstrs;
    QStringList rawInstrs;
    QMap<int, int> lineMapping;
        rawInstrs = getActiveFileContent(MainWindow::projectMainFile).remove(QRegExp("#[^\n]*")).split("\n");
        textInstrs = stripContent(getActiveFileContent(MainWindow::projectMainFile), lineMapping);
        lastTextInstrs = textInstrs;

    if (MainWindow::projectDataFile.trimmed() != ""){
        dataInstrs = stripContent(getActiveFileContent(MainWindow::projectDataFile), tempMap);
        lastDataInstrs = dataInstrs;
    }
    ui->actionAssemble->setEnabled(false);
    ui->actionSimulate->setEnabled(false);
    ui->actionAssemble_and_Simulate->setEnabled(false);
    ui->actionDelayedSimulation->setEnabled(false);
    ui->actionStepSimulation->setEnabled(false);
    if(assemblerInitialized || assem){
        ui->tableLog->clearContents();
        ui->tableLog->setRowCount(0);
        if (assem){
            simulationThread.quit();
            simulationThread.wait();
            pauseSimulation();
        }

    }
    ui->tabsProject->setCurrentIndex(1);
    assem->reset();
    assem->setBPs(codeArea->getMainBPs());
    assem->setStepped(stepped);
    assem->setSimulationSpeed(speed);
    assem->setRawList(rawInstrs);


    assemblerInitialized = false;

    QObject::connect(this, SIGNAL(assembleSignal(QStringList,QStringList)), assem, SLOT(assemble(QStringList,QStringList)), Qt::UniqueConnection);
    QObject::connect(assem, SIGNAL(progressUpdate(int)), this, SLOT(assemblingProgress(int)), Qt::UniqueConnection);
    QObject::connect(assem, SIGNAL(assemblyComplete()), this, SLOT(assemblyComplete()), Qt::UniqueConnection);
    QObject::connect(assem, SIGNAL(sendErrorMessage(int, QString, QString)), this, SLOT(appendErrorMessage(int, QString, QString)), Qt::UniqueConnection);

    simulationThread.start();
    simulationThread.setPriority(QThread::HighPriority);
    assembling = true;
    emit assembleSignal(dataInstrs, textInstrs);
    if (simulationBar == NULL)
        simulationBar = new QProgressBar(this);
    statusBar()->show();
    simulationBar->setValue(0);
    statusBar()->addWidget(simulationBar);
}

void MainWindow::getProjectLabels(bool checkClosed){

    if (!projectFile.isOpen())
        return;


    if (checkClosed){
        globalLabels.clear();
        if (MainWindow::projectDataFile.trimmed() != ""){
            globalLabels.append(getFileLabels(MainWindow::projectDataFile, checkClosed));
        }
        foreach(QString fileName, MainWindow::projectTextFiles){
            globalLabels.append(getFileLabels(fileName, checkClosed));
        }

    }else{
        QStringList openFiles;
        foreach(QMdiSubWindow *window, codeArea->subWindowList()){
            CodeEditorWindow *currentWindow = dynamic_cast<CodeEditorWindow *>(window);
            if (currentWindow){
                openFiles << currentWindow->getTitle();
            }
        }
        foreach(QString openFile, openFiles){
            globalLabels.append(getFileLabels(openFile, checkClosed));
        }

    }



    globalLabels.removeDuplicates();
    globalLabels.removeAll("");

}

QStringList MainWindow::getFileLabels(QString file, bool checkClosed){
    QStringList contentList = getActiveFileContent(file, checkClosed).split("\n");
    static QRegExp labelsRegEx("(\\S+)(?=:)");

    QStringList labelsList;
    foreach(QString line, contentList){
        if (line.trimmed() == "")
            continue;
        labelsRegEx.indexIn(line);
        labelsList << labelsRegEx.cap(0);
    }
    return labelsList;
}

void MainWindow::dropEvent(QDropEvent *e){
    const QMimeData *mime = e->mimeData();

    if(mime->hasUrls()){
        QList<QUrl> murls = mime->urls();
        if(murls.size() == 1){
            QString fileName = murls.at(0).toLocalFile();
            QString extension = QFileInfo(fileName).suffix();
            if (extension == "mpro"){
                QFile tempProject(fileName);
                if (!parseProjectXML(tempProject, false)){
                    return;
                }
                if(projectFile.isOpen()){
                    if(fileName == MainWindow::projectPath + MainWindow::projectFileName)
                        return;
                    if(QMessageBox::question(this, "Close Project", "Are you sure you want to close the active project?", QMessageBox::Yes, QMessageBox::No) != QMessageBox::Yes){
                        return;
                    }
                }
                openProjectFile(fileName);
            }else if(projectFile.isOpen()){
                if (extension == "mtxt"){
                    if(MainWindow::projectTextFiles.contains(QFileInfo(fileName).fileName().trimmed()))
                        return;
                    FileLoader *loader = new FileLoader(this, DROP_TEXT, fileName);
                    Q_UNUSED(loader);
                }else if (extension == "mdat"){
                    if(MainWindow::projectDataFile == QFileInfo(fileName).fileName().trimmed())
                        return;
                    FileLoader *loader = new FileLoader(this, DROP_DATA, fileName);
                    Q_UNUSED(loader);
                }else{
                    FileLoader *loader = new FileLoader(this, DROP_FILE);
                    loader->setFileName(fileName);
                    loader->exec();
                }
            }
        }
    }

}


bool MainWindow::eventFilter(QObject *o, QEvent *e){
    if (e->type() == QEvent::Show){
        ui->actionDefaultLayout->trigger();
        if(openWithArg.trimmed() == ""){
            StartupDialog *sdialog = new StartupDialog(this);
            sdialog->show();
            sdialog->setModal(true);
        }

    }

    return QMainWindow::eventFilter(o, e);
}

void MainWindow::closeEvent(QCloseEvent *e){
    if (closeAllWindows()){
        if (engine && engine->isVisible())
            engine->hide();
        e->accept();
        exit(0);
   } else
        e->ignore();

}


void MainWindow::openProjectAction()
{
    on_actionOpen_Project_triggered();
}

void MainWindow::createProjectAction()
{
    on_actionNew_Project_triggered();
}

void MainWindow::closeProject()
{
    if (closeAllWindows()){
        if (projectFile.isOpen()){
            projectFile.close();
        }
        setWindowTitle("Mirage");
        ui->treeFiles->clear();
        currentProjectString = "";
        MainWindow::projectPath = "";
        MainWindow::projectFileName = "";
        MainWindow::projectTitle = "";
        MainWindow::projectMainFile = "";
        MainWindow::projectTextFiles.clear();
        MainWindow::projectDataFile = "";
        MainWindow::projectConf.clear();
        pauseSimulation();
        codeArea->enableMainFileEditing();
    }
    refreshActions();

}


void MainWindow::addEditorWindow(QString file, QString title, MirageFileType type)
{
    CodeEditorWindow *editorWindow = new CodeEditorWindow(codeArea, editorFont, type);
    if(editorWindow->openFile(file, title)){
        QObject::connect(editorWindow->codeEditor(), SIGNAL(textChanged()), this, SLOT(getLabels()), Qt::UniqueConnection);
        codeArea->addSubWindow(editorWindow);
        if (MainWindow::projectMainFile.trimmed()!= "" && editorWindow->getTitle() == MainWindow::projectMainFile)
            codeArea->setMainWindow(editorWindow);
        editorWindow->codeEditor()->setContextMenuPolicy(Qt::CustomContextMenu);
        QObject::connect(editorWindow->codeEditor(), SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(editorWindowMenuRequested(QPoint)), Qt::UniqueConnection);
        QObject::connect(editorWindow->codeEditor(), SIGNAL(copyAvailable(bool)), this, SLOT(enableCopyCutDelete(bool)), Qt::UniqueConnection);
        QObject::connect(editorWindow->codeEditor(), SIGNAL(undoAvailable(bool)), this, SLOT(enableUndo(bool)), Qt::UniqueConnection);
        QObject::connect(editorWindow->codeEditor(), SIGNAL(redoAvailable(bool)), this, SLOT(enableRedo(bool)), Qt::UniqueConnection);
    }else{
        editorWindow->hide();
        QMessageBox::critical(this, "Error", "Failed to open the file " + file);
    }

}


void MainWindow::addDataFile(QString file){
    if (MainWindow::projectDataFile.trimmed() != ""){
        if (QMessageBox::question(this, "Data File Already Exists", "The project contains data file\nAre you sure you want to replace it?") != QMessageBox::Yes)
            return;
    }
    MainWindow::projectDataFile = file;
    rebuildProjectFile();
    loadProjectTree();
    QTreeWidgetItemIterator it(treeWidget);
    while (*it) {
        if ((*it)->text(0).trimmed() == file){
            on_treeFiles_itemDoubleClicked((*it), 0);
            break;
        }
        ++it;
    }
}

void MainWindow::addTextFile(QString file){
    if (MainWindow::projectTextFiles.contains(file)){
        QMessageBox::information(this, "Cannot load the text file", "A text file with the same name already exists in this project");
        return;
    }
    MainWindow::projectTextFiles.append(file);
    rebuildProjectFile();
    loadProjectTree();
    QTreeWidgetItemIterator it(treeWidget);
    while (*it) {
        if ((*it)->text(0).trimmed() == file){
            on_treeFiles_itemDoubleClicked((*it), 0);
            break;
        }
        ++it;
    }
}

void MainWindow::addResourceFile(QString file){

}

void MainWindow::removeDataFile(QString file){
    deleteConfirmed = false;
    deleteFromDisk = false;
    DeleteFileDialog *deleteDialog = new DeleteFileDialog(this);
    deleteDialog->setText("Are you sure you want to remove the file " + file + " from the project?");
    deleteDialog->exec();
    if (deleteConfirmed){
        if (deleteFromDisk){
            if (QMessageBox::warning(this, "Warning", "You are about to delete the file from the disk permanently\nAre you sure you want to proceed?", QMessageBox::Yes, QMessageBox::No) != QMessageBox::Yes)
                return;
            if (!QFile::remove(MainWindow::projectPath + file)){
                QMessageBox::critical(this, "Error", "Failed to remove the file from the disk");
                return;
            }
        }
        if (!closeFileWindow(file.trimmed())){
            QMessageBox::critical(this, "Error", "Cannot remove the file " + file);
            return;
        }
        MainWindow::projectDataFile = "";
        rebuildProjectFile();
        loadProjectTree();
    }
}


void MainWindow::removeTextFile(QString file){
    deleteConfirmed = false;
    deleteFromDisk = false;
    DeleteFileDialog *deleteDialog = new DeleteFileDialog(this);
    deleteDialog->setText("Are you sure you want to remove the file " + file + " from the project?");
    deleteDialog->exec();
    if (deleteConfirmed){
        if (deleteFromDisk){
            if (QMessageBox::warning(this, "Warning", "You are about to delete the file from the disk permanently\nAre you sure you want to proceed?", QMessageBox::Yes, QMessageBox::No) != QMessageBox::Yes)
                return;
            if (!QFile::remove(MainWindow::projectPath + file)){
                QMessageBox::critical(this, "Error", "Failed to remove the file from the disk");
                return;
            }
        }
        if (!closeFileWindow(file.trimmed())){
            QMessageBox::critical(this, "Error", "Cannot remove the file " + file);
            return;
        }
        for (int i = 0; i < MainWindow::projectTextFiles.size(); i++){
            if (MainWindow::projectTextFiles.at(i).trimmed() == file.trimmed()){
                MainWindow::projectTextFiles.removeAt(i);
                rebuildProjectFile();
                loadProjectTree();
                break;
            }

        }

    }
}

void MainWindow::removeMainTextFile(QString file){
    deleteConfirmed = false;
    deleteFromDisk = false;
    DeleteFileDialog *deleteDialog = new DeleteFileDialog(this);
    deleteDialog->setText("Are you sure you want to remove the file " + file + " from the project?");
    deleteDialog->exec();
    if (deleteConfirmed){
        if (deleteFromDisk){
            if (QMessageBox::warning(this, "Warning", "You are about to delete the file from the disk permanently\nAre you sure you want to proceed?", QMessageBox::Yes, QMessageBox::No) != QMessageBox::Yes)
                return;
            if (!QFile::remove(MainWindow::projectPath + file)){
                QMessageBox::critical(this, "Error", "Failed to remove the file from the disk");
                return;
            }
        }
        if (!closeFileWindow(file.trimmed())){
            QMessageBox::critical(this, "Error", "Cannot remove the file " + file);
            return;
        }
        MainWindow::projectMainFile = "";
        for (int i = 0; i < MainWindow::projectTextFiles.size(); i++){
            if (MainWindow::projectTextFiles.at(i).trimmed() == file.trimmed()){
                MainWindow::projectTextFiles.removeAt(i);
                rebuildProjectFile();
                loadProjectTree();
                break;
            }

        }

    }
}


void MainWindow::removeResourceFile(QString file){
    qDebug() << "Remove resource " << file;
}

void MainWindow::assemblingProgress(int value){
    if (simulationBar != NULL)
        simulationBar->setValue(value);
}

void MainWindow::simulationProgress(){
    static int simProg = 0;
    statusBar()->showMessage("Simulating" + QString(".").repeated(simProg++%3 + 1));
}


bool MainWindow::containsTextFile(QString textFileName) const{
    return MainWindow::projectTextFiles.contains(textFileName);
}

QString MainWindow::loadFileText(QString fileName){
    if(!projectFile.isOpen()){
        QMessageBox::critical(this, "Error", "Cannot load file, no open project");
        return "";
    }
    QFile file(MainWindow::projectPath + fileName);
    QString fileData;
    QTextStream stream (&file);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
        while (!stream.atEnd()){
            fileData.append(stream.readLine() + "\n");
        }
        file.close();
        return fileData;
    }else{
        QMessageBox::critical(this, "Error", "Failed to open the file for reading");
        return "";
    }

}

QString MainWindow::getProjectPath()
{
    return MainWindow::projectPath;
}

void MainWindow::setProjectPath(QString path)
{
    MainWindow::projectPath = path;
}

QString MainWindow::getProjectFileName(){
    return MainWindow::projectFileName;
}

QString MainWindow::getProjectTitle(){
    return MainWindow::projectTitle;
}


QString MainWindow::getProjectMainFile(){
    return MainWindow::projectMainFile;
}

QStringList MainWindow::getProjectTextFiles(){
    return MainWindow::projectTextFiles;
}

QString MainWindow::getProjectDataFile(){
    return MainWindow::projectDataFile;
}

QString MainWindow::getProjectConf(QString key){
    if (MainWindow::projectConf.contains(key))
        return MainWindow::projectConf[key];
    else
        return "";
}

bool MainWindow::isLittleEndian(){
    if (MainWindow::projectConf.contains("Endianness"))
        if (MainWindow::projectConf["Endianness"].toLower() == "big")
            return false;
    return true;
}

bool MainWindow::isGFXEnabled(){
    if (MainWindow::projectConf.contains("EnableGFX"))
        if (MainWindow::projectConf["EnableGFX"].toLower() == "true")
            return true;
    return false;
}



int MainWindow::getTileMapWidth(){
    if (MainWindow::projectConf.contains("TileMapWidth")){
        if (MainWindow::projectConf["TileMapWidth"].trimmed() == "3")
            return 3;
        else if (MainWindow::projectConf["TileMapWidth"].trimmed() == "2")
            return 2;
        else if (MainWindow::projectConf["TileMapWidth"].trimmed() == "2")
            return 1;
    }
    return 4;
}

int MainWindow::getTileMapHeight(){
    if (MainWindow::projectConf.contains("TileMapHeight")){
        if (MainWindow::projectConf["TileMapHeight"].trimmed() == "3")
            return 3;
        else if (MainWindow::projectConf["TileMapHeight"].trimmed() == "2")
            return 2;
        else if (MainWindow::projectConf["TileMapHeight"].trimmed() == "2")
            return 1;
    }
    return 4;
}

void MainWindow::resizeColumns(){
    resizeDataColumns();
    resizeTextColumns();
    resizeHeapColumns();
    resizeStackColumns();
    resizeRegsColumns();
    refreshMemoryModels();
}

bool MainWindow::hasOpenProject() const{
    return projectFile.isOpen();
}



MainWindow::~MainWindow(){
    if (projectFile.isOpen())
        projectFile.close();
    delete ui;
    simulationThread.quit();
    simulationThread.wait();
}

void MainWindow::resizeTextColumns(){
    ui->textTable->resizeColumnsToContents();

    if (ui->textTable->columnWidth(0) < minTextTableWidth0)
        ui->textTable->setColumnWidth(0, minTextTableWidth0);

    if (ui->textTable->columnWidth(1) < minTextTableWidth1)
        ui->textTable->setColumnWidth(1, minTextTableWidth1);
}

void MainWindow::resizeHeapColumns(){
    ui->heapTable->resizeColumnsToContents();

    if (ui->heapTable->columnWidth(0) < minHeapTableWidth0)
        ui->heapTable->setColumnWidth(0, minHeapTableWidth0);

    if (ui->heapTable->columnWidth(1) < minHeapTableWidth1)
        ui->heapTable->setColumnWidth(1, minHeapTableWidth1);
}

void MainWindow::resizeStackColumns(){
    ui->stackTable->resizeColumnsToContents();

    if (ui->stackTable->columnWidth(0) < minStackTableWidth0)
        ui->stackTable->setColumnWidth(0, minStackTableWidth0);

    if (ui->stackTable->columnWidth(1) < minStackTableWidth1)
        ui->stackTable->setColumnWidth(1, minStackTableWidth1);
}

void MainWindow::resizeRegsColumns(){
    ui->tableMainRegisters->resizeColumnsToContents();

    if (ui->tableMainRegisters->columnWidth(0) < minRegsTableWidth0)
        ui->tableMainRegisters->setColumnWidth(0, minRegsTableWidth0);

    if (ui->tableMainRegisters->columnWidth(1) < minRegsTableWidth1)
        ui->tableMainRegisters->setColumnWidth(1, minRegsTableWidth1);


}

void MainWindow::enableCopyCutDelete(bool value){
    ui->actionCopy->setEnabled(value);
    ui->actionCut->setEnabled(value);
    ui->actionPaste->setEnabled(value);
    ui->actionDeleteSelection->setEnabled(value);
}

void MainWindow::enableUndo(bool value){
    ui->actionUndo->setEnabled(value);
}

void MainWindow::enableRedo(bool value){
    ui->actionRedo->setEnabled(value);
}

void MainWindow::resizeDataColumns(){
    ui->dataTable->resizeColumnsToContents();

    if (ui->dataTable->columnWidth(0) < minDataTableWidth0)
        ui->dataTable->setColumnWidth(0, minDataTableWidth0);

    if (ui->dataTable->columnWidth(1) < minDataTableWidth1)
        ui->dataTable->setColumnWidth(1, minDataTableWidth1);

}

void MainWindow::projectExplorerMenuRequested(QPoint loc){
    if (!projectFile.isOpen())
        return;
    ExplorerTreeItem *itm = (ExplorerTreeItem*) ui->treeFiles->itemAt(loc);
    QMenu *menu=new QMenu(this);
    if (itm && (itm->getItemType() == TEXT_CHILD || itm->getItemType() == TEXT_MAIN || itm->getItemType() == DATA_CHILD)){
        QTreeWidgetItem *rawItem = ui->treeFiles->itemAt(loc);
        QAction *openAction = new QAction(this);
        QSignalMapper *openMapper = new QSignalMapper(this);
        openMapper->setMapping(openAction, (QObject *) rawItem);
        QObject::connect(openAction, SIGNAL(triggered()), openMapper, SLOT(map()), Qt::UniqueConnection);
        QObject::connect(openMapper, SIGNAL(mapped(QObject*)), this, SLOT(openTreeItem(QObject*)), Qt::UniqueConnection);
        openAction->setText("Open");
        menu->addAction(openAction);

        if (itm->getItemType() == TEXT_CHILD){
            QAction *setMainAction = new QAction(this);
            QSignalMapper *setMainMapper = new QSignalMapper(this);
            setMainMapper->setMapping(setMainAction, rawItem->text(0).trimmed());
            QObject::connect(setMainAction, SIGNAL(triggered()), setMainMapper, SLOT(map()), Qt::UniqueConnection);
            QObject::connect(setMainMapper, SIGNAL(mapped(QString)), this, SLOT(setMainProjectFile(QString)), Qt::UniqueConnection);
            setMainAction->setText("Set as Main File");
            menu->addAction(setMainAction);
        }else if (itm->getItemType() == TEXT_MAIN){
            QAction *unsetMainAction = new QAction(this);
            QObject::connect(unsetMainAction, SIGNAL(triggered()), this, SLOT(unsetMainProjectFile()), Qt::UniqueConnection);
            unsetMainAction->setText("Unset Main File");
            menu->addAction(unsetMainAction);
        }

        QAction *renameAction = new QAction(this);
        QSignalMapper *renameMapper = new QSignalMapper(this);
        renameMapper->setMapping(renameAction, (rawItem->text(0).trimmed()));
        QObject::connect(renameAction, SIGNAL(triggered()), renameMapper, SLOT(map()), Qt::UniqueConnection);
        renameAction->setText("Rename");

        QAction *removeAction = new QAction(this);
        QSignalMapper *removeMapper = new QSignalMapper(this);
        removeMapper->setMapping(removeAction, (rawItem->text(0).trimmed()));
        QObject::connect(removeAction, SIGNAL(triggered()), removeMapper, SLOT(map()), Qt::UniqueConnection);
        removeAction->setText("Remove");



        if (itm){
            if (itm->getItemType() == TEXT_CHILD){
                QObject::connect(renameMapper, SIGNAL(mapped(QString)), this, SLOT(renameTextItem(QString)), Qt::UniqueConnection);
                QObject::connect(removeMapper, SIGNAL(mapped(QString)), this, SLOT(removeTextFile(QString)), Qt::UniqueConnection);
            }else if (itm->getItemType() == TEXT_MAIN){
                QObject::connect(renameMapper, SIGNAL(mapped(QString)), this, SLOT(renameMainTextItem(QString)), Qt::UniqueConnection);
                QObject::connect(removeMapper, SIGNAL(mapped(QString)), this, SLOT(removeMainTextFile(QString)), Qt::UniqueConnection);
            }else if (itm->getItemType() == DATA_CHILD){
                QObject::connect(renameMapper, SIGNAL(mapped(QString)), this, SLOT(renameDataItem(QString)), Qt::UniqueConnection);
                QObject::connect(removeMapper, SIGNAL(mapped(QString)), this, SLOT(removeDataFile(QString)), Qt::UniqueConnection);
            }
        }

        menu->addAction(renameAction);
        menu->addAction(removeAction);

    }else{
        menu->addAction(ui->actionNew);
        menu->addAction(ui->actionOpen);
    }
    if (itm &&(itm->getItemType() == PROJECT_TITLE)){
        QAction *reconfAction = new QAction("Reconfigure", this);
        QObject::connect(reconfAction, SIGNAL(triggered()), this, SLOT(reconfigureProject()), Qt::UniqueConnection);
        menu->addAction(reconfAction);
        menu->addAction(ui->actionClose);
    }


    menu->popup(ui->treeFiles->viewport()->mapToGlobal(loc));
}

void MainWindow::editorWindowMenuRequested(QPoint loc){
    CodeEditorWindow *activeWindow = dynamic_cast<CodeEditorWindow *>(codeArea->activeSubWindow());
    if (activeWindow){
        QMenu *menu=new QMenu(this);
        menu->addAction(ui->actionUndo);
        menu->addAction(ui->actionRedo);
        menu->addSeparator();
        menu->addAction(ui->actionCut);
        menu->addAction(ui->actionCopy);
        menu->addAction(ui->actionPaste);
        menu->addAction(ui->actionDeleteSelection);
        menu->addSeparator();
        menu->addAction(ui->actionSelect_All);
        menu->popup(activeWindow->codeEditor()->viewport()->mapToGlobal(loc));
    }
}

void MainWindow::consoleMenuRequested(QPoint loc){
    QMenu *menu=new QMenu(this);
    QAction *exportConsoleAction = new QAction("Export", this);
    QAction *copyConsoleAction = new QAction ("Copy", this);
    QAction *copyAllConsoleAction = new QAction("Copy All", this);
    QAction *clearConsoleAction = new QAction("Clear", this);
    QObject::connect(exportConsoleAction, SIGNAL(triggered()), this, SLOT(on_btnExportConsole_clicked()), Qt::UniqueConnection);
    QObject::connect(copyConsoleAction, SIGNAL(triggered()), console, SLOT(copy()), Qt::UniqueConnection);
    QObject::connect(copyAllConsoleAction, SIGNAL(triggered()), console, SLOT(copyAll()), Qt::UniqueConnection);
    QObject::connect(clearConsoleAction, SIGNAL(triggered()), console, SLOT(clearConsole()), Qt::UniqueConnection);
    menu->addAction(exportConsoleAction);
    menu->addAction(copyConsoleAction);
    menu->addAction(copyAllConsoleAction);
    menu->addAction(clearConsoleAction);
    menu->popup(console->viewport()->mapToGlobal(loc));
}

void MainWindow::resumeSimulation(bool resume = true)
{

    QObject::connect(assem, SIGNAL(pauseRequest()), this, SLOT(pauseSimulation()), (Qt::ConnectionType)((Qt::ConnectionType)(Qt::BlockingQueuedConnection | Qt::UniqueConnection)));
    QObject::connect(this, SIGNAL(simulateSignal()), assem, SLOT(simulate()), Qt::UniqueConnection);
    QObject::connect(this, SIGNAL(resumeSimulationSignal()), assem, SLOT(resumeSimulation()), Qt::UniqueConnection);
    QObject::connect(assem, SIGNAL(setReadingLimit(int)), console, SLOT(setReadingLimit(int)), (Qt::ConnectionType)(Qt::BlockingQueuedConnection | Qt::UniqueConnection));
    QObject::connect(assem, SIGNAL(inputRequired(int)), this, SLOT(waitingInput()), Qt::UniqueConnection);
    QObject::connect(console, SIGNAL(sendChar(QString)), this, SLOT(inputReceived()), Qt::UniqueConnection);
    QObject::connect(console, SIGNAL(sendInt(int)), this, SLOT(inputReceived()), Qt::UniqueConnection);
    QObject::connect(console, SIGNAL(sendString(QString)), this, SLOT(inputReceived()), Qt::UniqueConnection);
    QObject::connect(assem, SIGNAL(simulationComplete(int)), this, SLOT(simulationComplete(int)), Qt::UniqueConnection);
    QObject::connect(assem, SIGNAL(printToConsole(QString)), this, SLOT(printToConsole(QString)), (Qt::ConnectionType)(Qt::BlockingQueuedConnection | Qt::UniqueConnection));
    QObject::connect(assem, SIGNAL(inputRequired(int)), console, SLOT(inputRequest(int)), (Qt::ConnectionType)(Qt::BlockingQueuedConnection | Qt::UniqueConnection));
    QObject::connect(console, SIGNAL(sendChar(QString)), assem, SLOT(readCharacter(QString)), (Qt::ConnectionType)(Qt::BlockingQueuedConnection | Qt::UniqueConnection));
    QObject::connect(console, SIGNAL(sendInt(int)), assem, SLOT(readInt(int)), (Qt::ConnectionType)(Qt::BlockingQueuedConnection | Qt::UniqueConnection));
    QObject::connect(console, SIGNAL(sendString(QString)), assem, SLOT(readString(QString)), Qt::BlockingQueuedConnection);
    QObject::connect(assem, SIGNAL(instructionExecuted()), this, SLOT(refreshModels()), (Qt::ConnectionType)(Qt::BlockingQueuedConnection | Qt::UniqueConnection));
    QObject::connect(assem, SIGNAL(pauseRequest()), this, SLOT(pauseSimulation()), (Qt::ConnectionType)(Qt::BlockingQueuedConnection | Qt::UniqueConnection));
    QObject::connect(assem, SIGNAL(executingLine(int)), this, SLOT(selectLine(int)), (Qt::ConnectionType)(Qt::BlockingQueuedConnection | Qt::UniqueConnection));

    simulating = true;
    simulationPaused = false;
    simulationStopped = false;
    refreshActions();
    if (!simulationThread.isRunning())
        simulationThread.start();
    timer->start();
    if (console->toPlainText().trimmed() != "" && !resume)
        console->addText("\n", true);
    if (resume){
        if (assem->isWaiting())
            console->enableEditing(true);
        emit resumeSimulationSignal();
    }else
        emit simulateSignal();

    statusBar()->showMessage("Simulating");
}

void MainWindow::refreshModels(){
    regModel->emitDataChanged();
    refreshMemoryModels();
}

void MainWindow::refreshMemoryModels(){
    regModel->emitDataChanged();
    textModel->emitDataChanged();
    dataModel->emitDataChanged();
    stackModel->emitDataChanged();
    heapModel->emitDataChanged();
    if (isGFXEnabled()){
        tmModel->emitDataChanged();
        bgtsModel->emitDataChanged();
        sptsModel->emitDataChanged();
        spRamModel->emitDataChanged();
        paletteModel->emitDataChanged();
        inputModel->emitDataChanged();
    }
}

void MainWindow::selectLine(int lineNumber){
    codeArea->selectMainFileLine(lineNumber);
}

void MainWindow::getLabels(){
    QStringList tempLabels(globalLabels);
    getProjectLabels(false);
    if (tempLabels != globalLabels)
        codeArea->setLabels(globalLabels);
}

QString MainWindow::getActiveFileContent(QString file, bool checkFiles){
    QList<QMdiSubWindow *> windows = codeArea->subWindowList();
    QString dataFileName = MainWindow::projectPath + file;
    foreach(QMdiSubWindow* currentWindow, windows){
        CodeEditorWindow *ceWindow = dynamic_cast<CodeEditorWindow *> (currentWindow);
        if (ceWindow){
            if (ceWindow->getFilePath() == dataFileName)
                return ceWindow->getContent();
        }
    }

    if (!checkFiles)
        return "";

    return loadFileText(file);
}



void MainWindow::on_actionSimulate_triggered(){

    if (!assemblerInitialized){
        if (QMessageBox::question(this, "Assemble Project", "Could not detect assembled data, do you want to assemble the project first?", QMessageBox::No, QMessageBox::Yes) == QMessageBox::Yes){
            on_actionAssemble_and_Simulate_triggered();
        }

        return;
    }


    if (lastTextInstrs.isEmpty())
        return;


    QStringList tempTextInstrs = stripContent(getActiveFileContent(MainWindow::projectMainFile), tempMap);
    QStringList tempDataInstrs = stripContent(getActiveFileContent(MainWindow::projectDataFile), tempMap);
    if (tempTextInstrs != lastTextInstrs || tempDataInstrs != lastDataInstrs){
        if (QMessageBox::question(this, "Re-assemble", "The assembled files have been modified, do you want to reassemble?", QMessageBox::No, QMessageBox::Yes) == QMessageBox::Yes){
            on_actionAssemble_and_Simulate_triggered();
        }

        return;

    }


    qDebug() << "Simulating..";
    if (engine != NULL)
        if(engine){
            if(engine->isVisible())
                engine->hide();
        }

    if(ui->actionEnable_Graphics_Engine->isChecked())
        engine->show();



    if (assemblerInitialized && assem){
        QTreeWidgetItemIterator itMain(treeWidget);
        while (*itMain) {
            if ((*itMain)->text(0).trimmed() == MainWindow::getProjectMainFile()){
                on_treeFiles_itemDoubleClicked((*itMain), 0);
                break;
            }
            ++itMain;
        }
        CodeEditorWindow *mainWindow = dynamic_cast<CodeEditorWindow *>(codeArea->activeSubWindow());
        if (mainWindow){
            mainWindow->disableEditing();
            QObject::connect(assem, SIGNAL(simulationComplete(int)), mainWindow, SLOT(enableEditing()), Qt::UniqueConnection);
        }


        ui->actionAssemble->setEnabled(false);
        ui->actionSimulate->setEnabled(false);
        ui->actionAssemble_and_Simulate->setEnabled(false);        
        ui->actionStepSimulation->setEnabled(false);
        ui->actionDelayedSimulation->setEnabled(false);
        resumeSimulation(false);
    }


}

void MainWindow::on_actionNew_triggered(){
    FileLoader *loader = new FileLoader(this, CREATE_FILE);
    loader->show();
}


void MainWindow::on_actionAssemble_triggered(){
    simulationSpeed = 0;
    assembleAction(simulationSpeed);
}

void MainWindow::on_actionClose_triggered(){
    if (!projectFile.isOpen())
        return;
    if(QMessageBox::question(this, "Close Project", "Are you sure you want to close the active project?", QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes){
        closeProject();
    }
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

}



void MainWindow::on_actionReload_Tiles_Memory_triggered()
{

    if (memoryLoading != NULL){
        if (memoryLoading->isVisible()){
            memoryLoading->show();
            return;
        }
    }

    memoryLoading = new MemoryLoading(0, this->memory);
    memoryLoading->setModal(true);
    memoryLoading->show();
}

void MainWindow::on_actionAssemble_and_Simulate_triggered()
{
    simulateAfterAssembling = true;
    ui->actionAssemble->trigger();
}

void MainWindow::on_actionSprite_Editor_triggered()
{
    spriteViewer = new SpriteViewer(this, memory);
    spriteViewer->show();
}

void MainWindow::on_actionOpen_Project_triggered()
{
    if (projectFile.isOpen())
        if(QMessageBox::question(this, "Close Project", "Are you sure you want to close the active project?", QMessageBox::Yes, QMessageBox::No) != QMessageBox::Yes)
            return;



    QString tempProjectFileName = QFileDialog::getOpenFileName(this, "Open Project", QDir::currentPath() /* + "/projects/"*/, "Mirage Project File(*.mpro)");
    if (tempProjectFileName.trimmed() == "")
        return;

    if (projectFile.isOpen())
        closeProject();
    openProjectFile(tempProjectFileName);

}



bool MainWindow::parseProjectXML(QFile &data, bool load = true){
    if(!data.open( QIODevice::ReadOnly | QIODevice::Text ) ){
        QMessageBox::critical(this, "Error", QString("Failed to open project file") + QString("\n ") + data.errorString());
        qDebug() << "Failed to open!";
    }else{

        QDomDocument domDocument;
        if(!domDocument.setContent(&data)){
            QMessageBox::critical(this, "Error", "Invalid project file");
            qDebug() << "Cannot set content";
            return false;
        }

        data.close();



        QDomElement domElement = domDocument.documentElement();
        if (domElement.tagName().trimmed() != "MirageProject"){
            QMessageBox::critical(this, "Error", "Inad project file");
            return false;
        }
        QDomNode child = domElement.firstChild();
        while(!child.isNull()) {
            QDomElement e = child.toElement();
            if (e.tagName().trimmed() == "ProjectTitle"){
                tempProjectTitle = e.toElement().text().trimmed();
            }else if (e.tagName().trimmed() == "TextSegment"){
                QDomNodeList textL = e.childNodes();
                tempProjectTextFiles.clear();
                for (int i = 0; i < textL.size(); i++){
                    QString tempTag = textL.at(i).toElement().tagName().trimmed();
                    QString tempTagValue = textL.at(i).toElement().text().trimmed();

                    if (tempTag == "MainFile"){
                        tempProjectMainFile = tempTagValue;
                        tempProjectTextFiles.append(tempTagValue);
                    }else if (tempTag == "File"){
                        tempProjectTextFiles.append(tempTagValue);
                    }
                }
            }else if (e.tagName().trimmed() == "DataSegment"){
                tempProjectDataFile = e.firstChild().toElement().text().trimmed();
            }else if (e.tagName().trimmed() == "Configure"){
                QDomNodeList confL = e.childNodes();
                tempProjectConf.clear();
                for (int i = 0; i < confL.size(); i++){
                    tempProjectConf[confL.at(i).toElement().tagName().trimmed()] = confL.at(i).toElement().text().trimmed();
                }
            }else{
                QMessageBox::critical(this, "Error", "Unidentified element " + e.tagName().trimmed() + " in the project file");
                return false;
            }
            child = child.nextSibling();

        }
        if (load){
            MainWindow::projectTitle =  tempProjectTitle;
            MainWindow::projectMainFile =  tempProjectMainFile;
            MainWindow::projectTextFiles = tempProjectTextFiles;
            MainWindow::projectDataFile = tempProjectDataFile;
            MainWindow::projectConf = tempProjectConf;
        }
        return true;
    }
    qDebug() << "Failed to parse";
    return false;

}

void MainWindow::loadProjectTree()
{
    treeWidget->clear();
    ExplorerTreeItem *projectItem = new ExplorerTreeItem(treeWidget, PROJECT_TITLE);
    QFont currentProjectFont = projectItem->font(0);
    currentProjectFont.setBold(true);
    projectItem->setText(0, MainWindow::projectTitle);
    projectItem->setFont(0, currentProjectFont);
    treeWidget->addTopLevelItem(projectItem);

    ExplorerTreeItem *textSegmentParent = new ExplorerTreeItem(projectItem, TEXT_ROOT);
    textSegmentParent->setText(0, "Text");
    projectItem->addChild(textSegmentParent);
    foreach(const QString &textFile, MainWindow::projectTextFiles){
        if (textFile.trimmed() == "") continue;
        ExplorerTreeItem *textFileItem = new ExplorerTreeItem(textSegmentParent, TEXT_CHILD);
        textFileItem->setText(0, textFile);
        textFileItem->setIcon(0, QIcon(":/icons/explorer/icons/explorer/textsegmentFileIcon.png"));
        if (textFile == MainWindow::projectMainFile){
            QFont currentItemFont = textFileItem->font(0);
            currentItemFont.setItalic(true);
            textFileItem->setItemType(TEXT_MAIN);
            textFileItem->setFont(0, currentItemFont);
        }
        textSegmentParent->addChild(textFileItem);
    }

    ExplorerTreeItem *dataSegmentParent = new ExplorerTreeItem(projectItem, DATA_ROOT);
    dataSegmentParent->setText(0, "Data");
    projectItem->addChild(dataSegmentParent);
    if (MainWindow::projectDataFile.trimmed() != ""){
        ExplorerTreeItem *dataFileItem = new ExplorerTreeItem(dataSegmentParent, DATA_CHILD);
        dataFileItem->setText(0, MainWindow::projectDataFile);
        dataFileItem->setIcon(0, QIcon(":/icons/explorer/icons/explorer/dataFileIcon.png"));
        dataSegmentParent->addChild(dataFileItem);
    }

    projectItem->setExpanded(true);
    textSegmentParent->setExpanded(true);
    dataSegmentParent->setExpanded(true);
}

bool MainWindow::validateProjectFiles(bool forceAll = true)
{
    if (MainWindow::projectTextFiles.size() > 0)
        foreach(const QString &textFile, MainWindow::projectTextFiles){
            if (textFile.trimmed() == "") continue;
            QFile testFile(MainWindow::projectPath + textFile);
            if (!testFile.open(QIODevice::ReadOnly | QIODevice::Text)){
                QMessageBox::critical(this, "Error", QString("Failed to load the file ") + textFile  + QString("\n") + testFile.errorString());
                if(!forceAll)
                    return false;
            }else{
                testFile.close();
            }
        }
    if (MainWindow::projectDataFile.trimmed() != ""){
        QFile testFile(MainWindow::projectPath + MainWindow::projectDataFile);
        if (!testFile.open(QIODevice::ReadOnly | QIODevice::Text)){
            QMessageBox::critical(this, "Error", QString("Failed to load the file ") + projectDataFile + QString("\n ") + testFile.errorString());
            if(!forceAll)
                return false;
        }else{
            testFile.close();
        }
    }



    return true;
}

bool MainWindow::validateTempProjectFiles(bool forceAll = true)
{
    if (MainWindow::projectTextFiles.size() > 0)
        foreach(const QString &textFile, tempProjectTextFiles){
            if (textFile.trimmed() == "") continue;
            QFile testFile(tempProjectPath + textFile);
            if (!testFile.open(QIODevice::ReadOnly | QIODevice::Text)){
                QMessageBox::critical(this, "Error", QString("Failed to load the file ") + textFile  + QString("\n") + testFile.errorString());
                if(!forceAll)
                    return false;
            }else{
                testFile.close();
            }
        }
    if (tempProjectDataFile.trimmed() != ""){
        QFile testFile(tempProjectPath + tempProjectDataFile);
        if (!testFile.open(QIODevice::ReadOnly | QIODevice::Text)){
            QMessageBox::critical(this, "Error", QString("Failed to load the file ") + projectDataFile + QString("\n ") + testFile.errorString());
            if(!forceAll)
                return false;
        }else{
            testFile.close();
        }
    }


    return true;
}

bool MainWindow::closeAllWindows(){
    QList<QMdiSubWindow *> subWindows = codeArea->subWindowList();
    foreach (QMdiSubWindow *currentWindow, subWindows){
        CodeEditorWindow *editorWindow = dynamic_cast<CodeEditorWindow*>(currentWindow);
        if (editorWindow){
            if (editorWindow->isEdited()){
                QMessageBox::StandardButton btn = QMessageBox::question(this, QString("Close"), QString("The changes to the file " + editorWindow->getTitle().trimmed() + " have not be saved\nDo you want to save the file before closing?"), QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, QMessageBox::Yes);
                if (btn == QMessageBox::Yes){
                    editorWindow->saveFile();
                }else if (btn != QMessageBox::No)
                    return false;
            }
        }
    }

    foreach (QMdiSubWindow *currentWindow, subWindows){
        CodeEditorWindow *editorWindow = dynamic_cast<CodeEditorWindow*>(currentWindow);
        if (editorWindow)
            editorWindow->setDestryoed(true);
    }

    codeArea->closeAllSubWindows();
    return true;

}

void MainWindow::openProjectFile(QString tempProjectFileName)
{

    tempProjectPath = QFileInfo(tempProjectFileName).absolutePath() + "/";
    tempProjectFileName = QFileInfo(tempProjectFileName).fileName();

    QFile tempProjectFile(tempProjectPath + tempProjectFileName);
    QString tempCurrentProjectString = "";
    QString line;
    QTextStream stream(&tempProjectFile);

    if (tempProjectFile.open(QIODevice::ReadOnly | QIODevice::ReadOnly)){
        if (!closeAllWindows())
            return;
        tempCurrentProjectString = "";
        while (!stream.atEnd()){
            line = stream.readLine();
            tempCurrentProjectString.append(line + '\n');
        }

        tempProjectFile.close();
        if (parseProjectXML(tempProjectFile)){
            if(validateTempProjectFiles(false)){
                QString tempCurrentProjectFileName = projectFile.fileName();
                projectFile.setFileName(tempProjectFile.fileName());
                if(projectFile.open(QIODevice::ReadWrite | QIODevice::Text)){
                    MainWindow::projectFileName = tempProjectFileName;
                    MainWindow::projectPath = tempProjectPath;
                    currentProjectString = tempCurrentProjectString;
                    loadProjectTree();
                    if (simulating){
                        pauseSimulation();
                    }
                    assemblerInitialized = false;
                    assembling = false;
                    simulating = false;
                    simulateAfterAssembling = false;
                    simulationPaused = true;
                    simulationSpeed = 0;

                    applyProjectSettings();

                    getProjectLabels(true);
                    codeArea->setLabels(globalLabels);

                    QTreeWidgetItemIterator it2(treeWidget);
                    while (*it2) {
                        if ((*it2)->text(0).trimmed() == MainWindow::getProjectMainFile()){
                            on_treeFiles_itemDoubleClicked((*it2), 0);
                            break;
                        }
                        ++it2;
                    }
                    setWindowTitle("Mirage - " + tempProjectTitle);
                }else{
                    projectFile.setFileName(tempCurrentProjectFileName);
                }
            }

        }
    }else{
        QMessageBox::critical(this, "Error", QString("Failed to open project file") + QString("\n ") + tempProjectFile.errorString());
        qDebug() << "Failed to open!";
    }
    refreshActions();
    memory->resizeTileMap();
}

void MainWindow::applyProjectSettings(){
    if(projectFile.isOpen()){
        if(MainWindow::projectConf.contains("EnableGFX") && MainWindow::projectConf["EnableGFX"] == "true")
            ui->actionEnable_Graphics_Engine->setChecked(true);
        else
            ui->actionEnable_Graphics_Engine->setChecked(false);
        refreshGraphicsAction();
    }

}

bool MainWindow::hasDataFile() const{
    return projectDataFile.trimmed() != "";

}

void MainWindow::setOpenWith(QString openWith){
    openWithArg = openWith;
    openProjectFile(openWith);
}

void MainWindow::setDeleteConfirmed(bool value){
    deleteConfirmed = value;
}

void MainWindow::setDeleteFromDisk(bool value){
    deleteFromDisk = value;
}

bool MainWindow::closeFileWindow(QString fileName){
    QList<QMdiSubWindow *> windows = codeArea->subWindowList();
    foreach (QMdiSubWindow *currentWindow, windows){
        CodeEditorWindow *editorWindow = dynamic_cast<CodeEditorWindow *>(currentWindow);
        if (editorWindow){
            if(editorWindow->getTitle().trimmed() == fileName.trimmed()){
                editorWindow->setDestryoed(true);
                return editorWindow->close();
            }
        }
    }
    return true;

}

void MainWindow::renameFileWindow(QString fileName, QString newName){
    QList<QMdiSubWindow *> windows = codeArea->subWindowList();
    foreach (QMdiSubWindow *currentWindow, windows){
        CodeEditorWindow *editorWindow = dynamic_cast<CodeEditorWindow *>(currentWindow);
        if (editorWindow){
            if(editorWindow->getTitle().trimmed() == fileName.trimmed()){
                editorWindow->setTitle(newName);
                return;
            }
        }
    }

}

void MainWindow::appendErrorMessage(int lineNumber, QString msg, QString segment){
    QTableWidgetItem *numItm = new QTableWidgetItem(QString::number(lineNumber));
    QTableWidgetItem *errorItm = new QTableWidgetItem(msg);
    QTableWidgetItem *segItm = new QTableWidgetItem(segment);

    ui->tableLog->setSortingEnabled(false);
    int itemPosition = ui->tableLog->rowCount();
    ui->tableLog->insertRow(itemPosition);
    ui->tableLog->setItem(itemPosition,0 , numItm);
    ui->tableLog->setItem(itemPosition, 1, errorItm);
    ui->tableLog->setItem(itemPosition, 2, segItm);

    ui->tableLog->setSortingEnabled(true);
    ui->tableLog->resizeColumnsToContents();
    ui->tableLog->resizeRowsToContents();
    ui->tabsProject->setCurrentIndex(2);

}

void MainWindow::disconnectSimulator()
{
    QObject::disconnect(this, SIGNAL(assembleSignal(QStringList,QStringList)), assem, SLOT(assemble(QStringList,QStringList)));
    QObject::disconnect(assem, SIGNAL(progressUpdate(int)), this, SLOT(assemblingProgress(int)));
    QObject::disconnect(assem, SIGNAL(assemblyComplete()), this, SLOT(assemblyComplete()));
    QObject::disconnect(this, SIGNAL(simulateSignal()), assem, SLOT(simulate()));
    QObject::disconnect(assem, SIGNAL(setReadingLimit(int)), console, SLOT(setReadingLimit(int)));
    QObject::disconnect(assem, SIGNAL(simulationComplete(int)), this, SLOT(simulationComplete(int)));
    QObject::disconnect(assem, SIGNAL(printToConsole(QString)), this, SLOT(printToConsole(QString)));
    QObject::disconnect(assem, SIGNAL(inputRequired(int)), console, SLOT(inputRequest(int)));
    QObject::disconnect(console, SIGNAL(sendChar(QString)), assem, SLOT(readCharacter(QString)));
    QObject::disconnect(console, SIGNAL(sendInt(int)), assem, SLOT(readInt(int)));
    QObject::disconnect(console, SIGNAL(sendString(QString)), assem, SLOT(readString(QString)));
    QObject::disconnect(assem, SIGNAL(inputRequired(int)), this, SLOT(waitingInput()));
    QObject::disconnect(console, SIGNAL(sendChar(QString)), this, SLOT(inputReceived()));
    QObject::disconnect(console, SIGNAL(sendInt(int)), this, SLOT(inputReceived()));
    QObject::disconnect(console, SIGNAL(sendString(QString)), this, SLOT(inputReceived()));
    QObject::disconnect(assem, SIGNAL(sendErrorMessage(int, QString, QString)), this, SLOT(appendErrorMessage(int,QString, QString)));
    QObject::disconnect(assem, SIGNAL(executingLine(int)), this, SLOT(selectLine(int)));
    QObject::disconnect(assem, SIGNAL(pauseRequest()), this, SLOT(pauseSimulation()));
    console->enableEditing(false);


    timer->stop();

    refreshModels();

    resizeColumns();
}

void MainWindow::pauseSimulation(){
    simulationPaused = true;
    disconnectSimulator();
    console->enableEditing(false);

    statusBar()->clearMessage();
    statusBar()->showMessage("Paused", 2000);



    refreshActions();
}



void MainWindow::addDefaultFont(){
    QFontDatabase fontsDB;
    fontsDB.addApplicationFont(":/font/consolas.ttf");
    if(fontsDB.families().contains("Consolas")){
        editorFont = fontsDB.font("Consolas", "Normal", 10);
    }
}

void MainWindow::on_actionInput_triggered(){
    inputManager = new InputManager(this, memory);
    inputManager->show();
}

void MainWindow::on_treeFiles_itemDoubleClicked(QTreeWidgetItem *item, int column){
    Q_UNUSED(column);
    if(item->parent()){
        QString parentItemText = item->parent()->text(0).trimmed();
        if (parentItemText == "Text" || parentItemText == "Data"){

            QString fileName = projectPath + item->text(0);

            foreach(QMdiSubWindow *window, codeArea->subWindowList()){
                if((dynamic_cast<CodeEditorWindow*> (window))->getFilePath() == fileName){
                    codeArea->setActiveSubWindow(window);
                    return;
                }
            }
            ExplorerTreeItem *itm = dynamic_cast<ExplorerTreeItem*>(item);
            if (!itm)
                return;
            if (itm->getItemType() == TEXT_CHILD || itm->getItemType() == TEXT_MAIN)
                addEditorWindow(fileName, item->text(0), TEXT_FILE);
            else if (itm->getItemType() == DATA_CHILD)
                addEditorWindow(fileName, item->text(0), DATA_FILE);
            else
                qDebug() << "Error: " << (dynamic_cast<ExplorerTreeItem*>(item))->getItemType();
            if(dynamic_cast<CodeEditorWindow*> (codeArea->activeSubWindow()))
                ((dynamic_cast<CodeEditorWindow*> (codeArea->activeSubWindow())))->setOpened();

        }
    }

}

void MainWindow::on_treeFiles_itemExpanded(QTreeWidgetItem *item){
    if (item->text(0) == "Text" || item->text(0) == "Data")
        item->setIcon(0, QIcon(":/icons/explorer/icons/explorer/folderIconOpened.png"));

}

void MainWindow::on_treeFiles_itemCollapsed(QTreeWidgetItem *item){
    if (item->text(0) == "Text" || item->text(0) == "Data")
        item->setIcon(0, QIcon(":/icons/explorer/icons/explorer/folderIconClosed.png"));
}

void MainWindow::on_actionViewProjectManager_triggered(){
    if(ui->actionViewProjectManager->isChecked())
        ui->dockProject->setVisible(true);
    else
        ui->dockProject->setVisible(false);

}

void MainWindow::on_actionViewAssemble_triggered(){
    if(ui->actionViewAssemble->isChecked())
        ui->dockAsm->setVisible(true);
    else
        ui->dockAsm->setVisible(false);
}

void MainWindow::on_actionViewCodeEditor_triggered(){
    if(ui->actionViewCodeEditor->isChecked())
        ui->dockCode->setVisible(true);
    else
        ui->dockCode->setVisible(false);
}

void MainWindow::on_actionViewMemory_triggered(){
    if(ui->actionViewMemory->isChecked())
        ui->dockMemory->setVisible(true);
    else
        ui->dockMemory->setVisible(false);
}

void MainWindow::on_actionOpen_triggered(){
    FileLoader *loader = new FileLoader(this, ADD_FILE);
    loader->show();
}

void MainWindow::openTreeItem(QObject *itm){
    this->on_treeFiles_itemDoubleClicked((QTreeWidgetItem *)itm, 0);
}

void MainWindow::activeWindowCopy(){
    CodeEditorWindow *activeWindow = dynamic_cast<CodeEditorWindow *>(codeArea->activeSubWindow());
    if (activeWindow){
        activeWindow->copy();
    }
}

void MainWindow::activeWindowCut(){
    CodeEditorWindow *activeWindow = dynamic_cast<CodeEditorWindow *>(codeArea->activeSubWindow());
    if (activeWindow){
        activeWindow->cut();
    }else
        qDebug() << "No active window";
}

void MainWindow::activeWindowPaste(){
    CodeEditorWindow *activeWindow = dynamic_cast<CodeEditorWindow *>(codeArea->activeSubWindow());
    if (activeWindow){
        activeWindow->paste();
    }
}

void MainWindow::activeWindowUndo(){
    CodeEditorWindow *activeWindow = dynamic_cast<CodeEditorWindow *>(codeArea->activeSubWindow());
    if (activeWindow){
        activeWindow->undo();
    }
}

void MainWindow::activeWindowRedo(){
    CodeEditorWindow *activeWindow = dynamic_cast<CodeEditorWindow *>(codeArea->activeSubWindow());
    if (activeWindow){
        activeWindow->redo();
    }
}

void MainWindow::activeWindowSelectAll()
{
    CodeEditorWindow *activeWindow = dynamic_cast<CodeEditorWindow *>(codeArea->activeSubWindow());
    if (activeWindow){
        activeWindow->selectAll();
    }
}

void MainWindow::activeWindowQuickFind()
{
    CodeEditorWindow *activeWindow = dynamic_cast<CodeEditorWindow *>(codeArea->activeSubWindow());
    if (activeWindow){
        activeWindow->quickFind();
    }
}

void MainWindow::activeWindowFindAndReplace()
{
    CodeEditorWindow *activeWindow = dynamic_cast<CodeEditorWindow *>(codeArea->activeSubWindow());
    if (activeWindow){
        activeWindow->findAndReplace();
    }
}

void MainWindow::activeWindowDelete(){
    CodeEditorWindow *activeWindow = dynamic_cast<CodeEditorWindow *>(codeArea->activeSubWindow());
    if (activeWindow){
        activeWindow->deleteText();
    }
}

void MainWindow::refreshActions(){
    bool value = projectFile.isOpen();
    if (!(assembling || simulating) && !(simulating && simulationPaused)){
        ui->actionAssemble->setEnabled(value);
        ui->actionAssemble_and_Simulate->setEnabled(value);
        ui->actionSimulate->setEnabled(value);
        ui->actionDelayedSimulation->setEnabled(value);
        ui->actionStepSimulation->setEnabled(value);
    }

    ui->actionPause_Simulation->setEnabled(simulating && !simulationPaused);
    ui->actionResumeSimulation->setEnabled(simulating && simulationPaused);

    ui->actionStepForwared->setEnabled(simulating && (simulationPaused));
    ui->actionStepBackward->setEnabled(simulating && (simulationPaused));
    ui->actionStopSimulation->setEnabled(simulating);

    ui->actionClose->setEnabled(value);
    ui->actionEnable_Graphics_Engine->setEnabled(value);
    ui->actionInput->setEnabled(value);
    ui->actionInsert_Breakpoint->setEnabled(value);
    ui->actionSaveAs->setEnabled(value);
    ui->actionNew->setEnabled(value);
    ui->actionOpen->setEnabled(value);
}

void MainWindow::refreshEditActions(){
    CodeEditorWindow *activeWindow = dynamic_cast<CodeEditorWindow *>(codeArea->activeSubWindow());
    if (!activeWindow){
        ui->actionCopy->setEnabled(false);
        ui->actionCut->setEnabled(false);
        ui->actionPaste->setEnabled(false);
        ui->actionUndo->setEnabled(false);
        ui->actionRedo->setEnabled(false);
        ui->actionDeleteSelection->setEnabled(false);
    }
    ui->actionFindandReplace->setEnabled(activeWindow);
    ui->actionQuickFind->setEnabled(activeWindow);
    ui->actionSave->setEnabled(activeWindow);
    ui->actionSelect_All->setEnabled(activeWindow);
}

void MainWindow::refreshGraphicsAction(){
    bool value = ui->actionEnable_Graphics_Engine->isChecked();
    if(ui->actionEnable_Graphics_Engine->isChecked()){
        ui->actionEnable_Graphics_Engine->setText("Disable Graphics Engine");
        ui->actionEnable_Graphics_Engine->setToolTip("Disable Graphics Engine");
    }else{
        ui->actionEnable_Graphics_Engine->setText("Enable Graphics Engine");
        ui->actionEnable_Graphics_Engine->setToolTip("Enable Graphics Engine");
    }
    ui->actionTileset_viewer->setEnabled(value);
    ui->actionPalette_Viewer->setEnabled(value);
    ui->actionReload_Tiles_Memory->setEnabled(value);
    ui->actionTile_loader->setEnabled(false);
    ui->actionSprite_Editor->setEnabled(value);
    ui->tabGFX->setEnabled(value);
}



void MainWindow::rebuildProjectFile(){
    if (projectFile.isOpen()){
        QFile tempProject(projectFile.fileName());
        if (tempProject.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)){
            QTextStream writer(&tempProject);
            writer << "<MirageProject>" << endl;
            writer << "<ProjectTitle>" << endl;
            writer << MainWindow::projectTitle << endl;
            writer << "</ProjectTitle>" << endl;
            writer << "<TextSegment>" << endl;
            if (MainWindow::projectMainFile.trimmed() != ""){
                writer << "<MainFile>" << endl;
                writer << MainWindow::projectMainFile.trimmed() << endl;
                writer << "</MainFile>" << endl;
            }
            foreach(QString fileName, MainWindow::projectTextFiles){
                if (fileName.trimmed() != MainWindow::projectMainFile.trimmed()){
                    writer << "<File>" << endl;
                    writer << fileName.trimmed() << endl;
                    writer << "</File>" << endl;
                }
            }
            writer << "</TextSegment>" << endl;
            writer << "<DataSegment>" << endl;
            if (MainWindow::projectDataFile.trimmed() != ""){
                writer << "<Data>" << endl;
                writer << MainWindow::projectDataFile.trimmed() << endl;
                writer << "</Data>" << endl;
            }
            writer << "</DataSegment>" << endl;
            writer << "<Configure>" << endl;
            writer << "<Endianness>" << endl;
            if (!MainWindow::isLittleEndian())
                writer << "big" << endl;
            else
                writer << "little" << endl;
            writer << "</Endianness>" << endl;
            if(MainWindow::isGFXEnabled()){
                writer << "<EnableGFX>" << endl;
                writer << "true" << endl;
                writer << "</EnableGFX>" << endl;
                writer << "<TileMapHeight>" << endl;
                writer << MainWindow::getTileMapHeight() << endl;
                writer << "</TileMapHeight>" << endl;
                writer << "<TileMapWidth>" << endl;
                writer << MainWindow::getTileMapWidth() << endl;
                writer << "</TileMapWidth>" << endl;

            }else{
                writer << "<EnableGFX>" << endl;
                writer << "false" << endl;
                writer << "</EnableGFX>" << endl;
                writer << "<TileMapHeight>" << endl;
                writer << "4" << endl;
                writer << "</TileMapHeight>" << endl;
                writer << "<TileMapWidth>" << endl;
                writer << "4" << endl;
                writer << "</TileMapWidth>" << endl;
            }
            writer << "</Configure>" << endl;
            writer << "</MirageProject>";
            tempProject.close();
        }else
            QMessageBox::critical(this, "Error", "Could not save the project file");
    }else
        QMessageBox::critical(this, "Error", "Could not save the project file");

}

void MainWindow::setMainProjectFile(QString file)
{
    if (!MainWindow::projectTextFiles.contains(file)){
        QMessageBox::critical(this, "Error", "Cannot find the specified text fie");
        return;
    }
    MainWindow::projectMainFile = file;
    rebuildProjectFile();
    loadProjectTree();
}

void MainWindow::unsetMainProjectFile(){
    MainWindow::projectMainFile = "";
    rebuildProjectFile();
    loadProjectTree();
}

void MainWindow::renameDataItem(QString file){
    QString newName = QInputDialog::getText(this, "Rename Data File", "Enter the new file name").trimmed();
    if (newName.trimmed() == "")
        return;
    if (QRegExp("^[A-Za-z0-9_\\@\\$\\.\\s]*$").indexIn(newName) == -1){
        QMessageBox::critical(this, "Invalid File Name", "The new file name contains invalid characters");
        return;
    }
    QFile renamedFile(MainWindow::projectPath + file);
    if (!renamedFile.rename(newName + ".mdat")){
        QMessageBox::critical(this, "Error", "Failed to rename the file");
        return;
    }
    renamedFile.close();
    MainWindow::projectDataFile = newName + ".mdat";
    rebuildProjectFile();
    loadProjectTree();
    renameFileWindow(file, newName + ".mdat");


}


void MainWindow::renameMainTextItem(QString file){

}


void MainWindow::renameTextItem(QString file){

}

void MainWindow::renameResItem(QString file){

}


void MainWindow::on_actionNew_Project_triggered(){
    ProjectCreator *builder = new ProjectCreator(this);
    builder->setModal(true);
    builder->show();

}

void MainWindow::on_actionDefaultLayout_triggered()
{
    ui->dockAsm->setVisible(true);
    ui->dockCode->setVisible(true);
    ui->dockMemory->setVisible(true);
    ui->dockProject->setVisible(true);

    removeDockWidget(ui->dockAsm);
    removeDockWidget(ui->dockCode);
    removeDockWidget(ui->dockMemory);
    removeDockWidget(ui->dockProject);

    addDockWidget(Qt::LeftDockWidgetArea, ui->dockCode);
    setCentralWidget(ui->dockCode);

    addDockWidget(Qt::LeftDockWidgetArea, ui->dockProject);
    addDockWidget(Qt::RightDockWidgetArea, ui->dockMemory);
    addDockWidget(Qt::LeftDockWidgetArea, ui->dockAsm);

    ui->dockAsm->setFloating(false);
    ui->dockCode->setFloating(false);
    ui->dockMemory->setFloating(false);
    ui->dockProject->setFloating(false);

    ui->dockAsm->setVisible(true);
    ui->dockCode->setVisible(true);
    ui->dockMemory->setVisible(true);
    ui->dockProject->setVisible(true);

}

void MainWindow::on_actionSave_triggered(){
    CodeEditorWindow *activeWin = dynamic_cast<CodeEditorWindow *> (codeArea->activeSubWindow());
    if (activeWin)
        activeWin->saveFile();

}


void MainWindow::on_actionEnable_Graphics_Engine_triggered(){
    if(ui->actionEnable_Graphics_Engine->isChecked())
        MainWindow::projectConf["EnableGFX"] = "true";
    else
        MainWindow::projectConf["EnableGFX"] = "false";

    rebuildProjectFile();
}



void MainWindow::simulationComplete(int code = 0){
    if (code == 0){
        simulating = false;
        simulationPaused = false;
    }
    disconnectSimulator();
    refreshActions();

    simulationThread.quit();
    simulationThread.wait();


    refreshModels();

    resizeColumns();

    statusBar()->clearMessage();
    if (code == 0){
        statusBar()->showMessage("Simulated!", 2000);
        qDebug() << "Simulated.";
    }else if (code == 1){
        statusBar()->showMessage("Breakpoint", 2000);
    }else if (code == 2){
        statusBar()->showMessage("Stepped", 500);
    }
}

void MainWindow::assemblyComplete(){

    qDebug() << "Assembled";
    assembling = false;
    refreshActions();
    assemblerInitialized = true;
    refreshModels();
    refreshMemoryModels();

    simulationThread.quit();
    simulationThread.wait();

    if (simulationBar != NULL){
        simulationBar->setValue(100);
        simulationBar->hide();
    }
    statusBar()->showMessage("Assembled", 2000);
    if (simulateAfterAssembling && !(assem->numberOfErrors() > 0))
        ui->actionSimulate->trigger();
    simulateAfterAssembling = false;
}

void MainWindow::on_btnClearLog_clicked(){
    ui->tableLog->clearContents();
    ui->tableLog->setRowCount(0);
}

void MainWindow::on_actionMemory_Dump_triggered()
{
    memoryDump = new MemoryDump(this, memory);
    memoryDump->show();
}

void MainWindow::on_actionGPU_Memory_Dump_triggered()
{
    gpuMemoryDump = new GpuMemoryDump(this, memory, this);
    gpuMemoryDump->show();
}

void MainWindow::on_btnExportConsole_clicked(){
    QString fileName;
    if (projectFile.isOpen()){
        fileName = QFileDialog::getSaveFileName(this, "Export Console", MainWindow::projectPath + "console.txt", "Text File(*.txt)");
    }else{
        fileName = QFileDialog::getSaveFileName(this, "Export Console", QDir::currentPath() + "/console.txt", "Text File(*.txt)");
    }
    if (fileName.trimmed() == "")
        return;

    QFile file(fileName);
    if(file.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Truncate)){
        QTextStream writer(&file);
        QStringList consoleText = console->toPlainText().split("\n");
        foreach(QString line, consoleText)
            writer << line << "\n";
        file.close();
    }else
        QMessageBox::critical(this, "Error", "Failed to open the file for writing");
}

void MainWindow::on_btnCopyConsole_clicked(){
    console->copyAll();
}

void MainWindow::on_btnClearConsole_clicked(){
    console->clearConsole();
}

void MainWindow::waitingInput(){
    timer->stop();
    statusBar()->showMessage("Waiting for input");
}

void MainWindow::inputReceived(){
    statusBar()->clearMessage();
    timer->start();
}

void MainWindow::printToConsole(QString msg){
    console->addText(msg, true);
}

void MainWindow::stopSimulation(){
    simulationStopped = true;
}

void MainWindow::openTilesetViewer(){
    on_actionTileset_viewer_triggered();
}

void MainWindow::openPaletteViewer(){
    on_actionPalette_Viewer_triggered();
}

bool MainWindow::isSimulationPaused() const{
    return simulationPaused;
}

bool MainWindow::isSimulationStopped() const{
    return simulationStopped;
}

void MainWindow::initMemoryModels(bool checkGFX = false){

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

    if (checkGFX)
        if(!ui->actionEnable_Graphics_Engine->isChecked())
            return;
    tmModel = new MemoryModel(memory, this, TMSegment, ui->tmAddressMode, ui->tmMemoryMode, ui->tmMemoryBase);
    bgtsModel = new MemoryModel(memory, this, BGTSSegment, ui->BGTSAddressMode, ui->BGTSMemoryMode, ui->BGTSMemoryBase);
    sptsModel = new MemoryModel(memory, this, SPTSSegment, ui->SPTSAddressMode, ui->SPRAMMemoryMode, ui->SPTSMemoryBase);
    spRamModel = new MemoryModel(memory, this, SPRamSegment, ui->SPRAMAddressMode, ui->SPRAMMemoryMode, ui->SPRAMMemoryBase);
    paletteModel = new MemoryModel(memory, this, PaletteSegment, ui->paletteAddressMode, ui->paletteMemoryMode, ui->paletteMemoryBase);
    inputModel = new MemoryModel(memory, this, InputSegment, ui->inputAddressMode, ui->inputMemoryMode, ui->inputMemoryBase);

    ui->tmTable->setModel(tmModel);
    ui->bgtsTable->setModel(bgtsModel);
    ui->sptsTable->setModel(sptsModel);
    ui->spRamTable->setModel(spRamModel);
    ui->paletteTable->setModel(paletteModel);
    ui->inputTable->setModel(inputModel);
}

void MainWindow::initRegs(){
    for (int i = 0; i < 35; i++){
        mainProcessorRegisters.append(0);
    }
    mainProcessorRegisters[28] = 0x10008000;
    mainProcessorRegisters[29] = 0x001FFFFC;
}

void MainWindow::connectActions(){
    QObject::connect(ui->actionEnable_Graphics_Engine, SIGNAL(toggled(bool)), this,SLOT(refreshGraphicsAction()), Qt::UniqueConnection);
    QObject::connect(ui->treeFiles, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(projectExplorerMenuRequested(QPoint)), Qt::UniqueConnection);
    QObject::connect(codeArea,SIGNAL(subWindowActivated(QMdiSubWindow*)), this, SLOT(refreshEditActions()));
    QObject::connect(ui->actionCopy, SIGNAL(triggered()), this, SLOT(activeWindowCopy()), Qt::UniqueConnection);
    QObject::connect(ui->actionCut, SIGNAL(triggered()), this, SLOT(activeWindowCut()), Qt::UniqueConnection);
    QObject::connect(ui->actionPaste, SIGNAL(triggered()), this, SLOT(activeWindowPaste()), Qt::UniqueConnection);
    QObject::connect(ui->actionUndo, SIGNAL(triggered()), this, SLOT(activeWindowUndo()), Qt::UniqueConnection);
    QObject::connect(ui->actionRedo, SIGNAL(triggered()), this, SLOT(activeWindowRedo()), Qt::UniqueConnection);
    QObject::connect(ui->actionSelect_All, SIGNAL(triggered()), this, SLOT(activeWindowSelectAll()), Qt::UniqueConnection);
    QObject::connect(ui->actionQuickFind, SIGNAL(triggered()), this, SLOT(activeWindowQuickFind()), Qt::UniqueConnection);
    QObject::connect(ui->actionFindandReplace, SIGNAL(triggered()), this, SLOT(activeWindowFindAndReplace()), Qt::UniqueConnection);
    QObject::connect(ui->actionDeleteSelection, SIGNAL(triggered()), this, SLOT(activeWindowDelete()), Qt::UniqueConnection);
    QObject::connect(ui->actionPause_Simulation, SIGNAL(triggered()), this, SLOT(pauseSimulation()), Qt::UniqueConnection);
    QObject::connect(this, SIGNAL(simulateSignal()), codeArea, SLOT(disableMainFileEditing()), Qt::UniqueConnection);
    QObject::connect(assem, SIGNAL(simulationComplete(int)), codeArea, SLOT(enableMainFileEditing())), Qt::UniqueConnection;
}

void MainWindow::setupColumnsResize(){
    minDataTableWidth0 = ui->dataTable->columnWidth(0);
    minDataTableWidth1 = ui->dataTable->columnWidth(1) + 10;
    minTextTableWidth0 = ui->textTable->columnWidth(0);
    minTextTableWidth1 = ui->textTable->columnWidth(1) + 10;
    minHeapTableWidth0 = ui->heapTable->columnWidth(0);
    minHeapTableWidth1 = ui->heapTable->columnWidth(1) + 10;
    minStackTableWidth0 = ui->stackTable->columnWidth(0);
    minStackTableWidth1 = ui->stackTable->columnWidth(1) + 10;
    minRegsTableWidth0 = ui->tableMainRegisters->columnWidth(0);
    minRegsTableWidth1 = ui->tableMainRegisters->columnWidth(1) + 10;
    resizeColumns();

    QObject::connect(ui->dataAddressMode, SIGNAL(currentIndexChanged(int)), this, SLOT(resizeDataColumns()), Qt::UniqueConnection);
    QObject::connect(ui->dataMemoryMode, SIGNAL(currentIndexChanged(int)), this, SLOT(resizeDataColumns()), Qt::UniqueConnection);
    QObject::connect(ui->dataMemoryBase, SIGNAL(currentIndexChanged(int)), this, SLOT(resizeDataColumns()), Qt::UniqueConnection);

    QObject::connect(ui->textAddressMode, SIGNAL(currentIndexChanged(int)), this, SLOT(resizeTextColumns()), Qt::UniqueConnection);
    QObject::connect(ui->textMemoryMode, SIGNAL(currentIndexChanged(int)), this, SLOT(resizeTextColumns()), Qt::UniqueConnection);
    QObject::connect(ui->textMemoryBase, SIGNAL(currentIndexChanged(int)), this, SLOT(resizeTextColumns()), Qt::UniqueConnection);

    QObject::connect(ui->heapAddressMode, SIGNAL(currentIndexChanged(int)), this, SLOT(resizeHeapColumns()), Qt::UniqueConnection);
    QObject::connect(ui->heapMemoryMode, SIGNAL(currentIndexChanged(int)), this, SLOT(resizeHeapColumns()), Qt::UniqueConnection);
    QObject::connect(ui->heapMemoryBase, SIGNAL(currentIndexChanged(int)), this, SLOT(resizeHeapColumns()), Qt::UniqueConnection);

    QObject::connect(ui->stackAddressMode, SIGNAL(currentIndexChanged(int)), this, SLOT(resizeStackColumns()), Qt::UniqueConnection);
    QObject::connect(ui->stackAddressMode, SIGNAL(currentIndexChanged(int)), this, SLOT(resizeStackColumns()), Qt::UniqueConnection);
    QObject::connect(ui->stackAddressMode, SIGNAL(currentIndexChanged(int)), this, SLOT(resizeStackColumns()), Qt::UniqueConnection);

    QObject::connect(ui->registersNaming, SIGNAL(currentIndexChanged(int)), this, SLOT(resizeRegsColumns()), Qt::UniqueConnection);
    QObject::connect(ui->registersBase, SIGNAL(currentIndexChanged(int)), this, SLOT(resizeRegsColumns()), Qt::UniqueConnection);
}

QStringList MainWindow::stripContent(QString rawText, QMap<int, int> &mapping){
    QStringList rawLines = rawText.split("\n");
    int rawIterator = 0;
    if (rawLines.size() == 0)
        return QStringList();
    QRegExp commentsRegEx("#[^\n]*");
    QStringList lines = rawText.remove(commentsRegEx).split("\n");
    for (int i = 0; i < lines.length(); i++)
        lines[i] = lines[i].trimmed();
    //lines.removeAll("");
    mapping.clear();
    for (int i = 0; i < lines.length(); i++){
        for(int j = rawIterator; j < rawLines.size(); j++){
            if(lines.at(i) == rawLines.at(j).trimmed()){
                mapping[i] = j;
                rawIterator = j + 1;
                break;
            }
        }
    }

    return lines;
}



void MainWindow::on_actionResumeSimulation_triggered(){
    resumeSimulation(true);
}

void MainWindow::on_actionStopSimulation_triggered(){
    /*pauseSimulation();
    simulationComplete();
    statusBar()->clearMessage();*/
    simulationStopped = true;
}

void MainWindow::on_actionBase_Converter_triggered()
{
    logicCalculator = new LogicCalculator(this);
    logicCalculator->show();
}

void MainWindow::on_actionDelayedSimulation_triggered(){
    QPointer<DealyedSimulationDialog> simDlg = new DealyedSimulationDialog(this);

    if (simDlg->exec() == QDialog::Accepted){
        simulationSpeed = simDlg->getSpeed();
        simulateAfterAssembling = true;
        assembleAction(simulationSpeed);
    }
}

void MainWindow::on_actionStepForwared_triggered(){
      if(simulating){
          assem->stepForward(true);
          resumeSimulation(true);
      }else
          qDebug() << "Not simulating";
}

void MainWindow::on_tableLog_doubleClicked(const QModelIndex &index){
    if (!index.isValid() || index.row() >= ui->tableLog->rowCount() || index.column() > 2)
        return;
    QTableWidgetItem *clickedItmSegment = ui->tableLog->item(index.row(), 2);
    QString clickedSegment = clickedItmSegment->text();

    QTableWidgetItem *clickedItmLine = ui->tableLog->item(index.row(), 0);
    int clickedLine = clickedItmLine->text().toInt();
    if (clickedSegment == "Data"){
        QTreeWidgetItemIterator it(treeWidget);
        while (*it) {
            if ((*it)->text(0).trimmed() == MainWindow::projectDataFile){
                on_treeFiles_itemDoubleClicked((*it), 0);
                break;
            }
            ++it;
        }
        CodeEditorWindow *dataWindow = dynamic_cast<CodeEditorWindow *>(codeArea->activeSubWindow());
        if (dataWindow){
            QTextCursor curs = dataWindow->codeEditor()->textCursor();
            curs.clearSelection();
            curs.setPosition(0);
            curs.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor, clickedLine);
            dataWindow->codeEditor()->setTextCursor(curs);
        }


    }else if (clickedSegment == "Text"){
        QTreeWidgetItemIterator it(treeWidget);
        while (*it) {
            if ((*it)->text(0).trimmed() == MainWindow::projectMainFile){
                on_treeFiles_itemDoubleClicked((*it), 0);
                break;
            }
            ++it;
        }

        CodeEditorWindow *textWindow = dynamic_cast<CodeEditorWindow *>(codeArea->activeSubWindow());
        if (textWindow){
            QTextCursor curs = textWindow->codeEditor()->textCursor();
            curs.clearSelection();
            curs.setPosition(0);
            curs.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor, clickedLine);
            textWindow->codeEditor()->setTextCursor(curs);
        }
    }
}

void MainWindow::on_actionInsert_Breakpoint_triggered(){
    CodeEditorWindow *activeWin = dynamic_cast<CodeEditorWindow *> (codeArea->activeSubWindow());
    if (activeWin)
        activeWin->toogleBP();
}

void MainWindow::on_actionClearBreakpoints_triggered(){
    CodeEditorWindow *activeWin = dynamic_cast<CodeEditorWindow *> (codeArea->activeSubWindow());
    if (activeWin)
        activeWin->clearBPs();
}

void MainWindow::on_actionStepSimulation_triggered(){
    simulateAfterAssembling = true;
    assembleAction(simulationSpeed, true);
}

void MainWindow::reconfigureProject(){
    recDialog->setParams(getProjectTitle(), isGFXEnabled(), isLittleEndian(), getTileMapWidth(), getTileMapHeight());
    int retCode = recDialog->exec();
    if (retCode == QDialog::Accepted){
        MainWindow::projectTitle = recDialog->getTitle();
        if (recDialog->getLEndianState())
            MainWindow::projectConf["Endianness"] = "little";
        else
            MainWindow::projectConf["Endianness"] = "big";
        if (recDialog->getGFXState())
            MainWindow::projectConf["EnableGFX"] = "true";
        else
            MainWindow::projectConf["EnableGFX"] = "false";


         MainWindow::projectConf["TileMapWidth"] = QString::number(recDialog->getTileWidth());
         MainWindow::projectConf["TileMapHeight"] = QString::number(recDialog->getTileHeight());

         rebuildProjectFile();

         applyProjectSettings();

         loadProjectTree();

         setWindowTitle("Mirage - " + MainWindow::projectTitle);

         memory->resizeTileMap();
    }




}
