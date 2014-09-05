#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeView>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QFile>
#include <QThread>
#include <QProgressBar>
#include <QTimer>

#include "syntaxhl.h"
#include "assembler.h"
#include "loadmemorythread.h"
#include "memoryloading.h"
#include "tilesetviewer.h"
#include "paletteviewer.h"
#include "tileloader.h"
#include "registersmodel.h"
#include "memorymodel.h"
#include "spriteviewer.h"
#include "inputmanager.h"
#include "codeeditorwindow.h"
#include "ioconsole.h"
#include "memorydump.h"
#include "gpumemorydump.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    bool eventFilter(QObject *, QEvent *);

    void openProjectAction();
    void createProjectAction();

    void closeProject();

    void addEditorWindow(QString file, QString title, MirageFileType type);

    void addDataFile(QString file);
    void addTextFile(QString file);
    void addResourceFile(QString file);

    bool containsTextFile(QString textFileName);


    QString loadFileText(QString);
    static QString getProjectPath();
    static void setProjectPath(QString);
    static QString getProjectFileName();
    static QString getProjectTitle();
    static QString getProjectMainFile();
    static QStringList getProjectTextFiles();
    static QString getProjectDataFile();
    static QString getProjectConf(QString);
    static bool isLittleEndian();
    static bool isGFXEnabled();
    static int getTileMapWidth();
    static int getTileMapHeight();

    void resizeColumns();

    bool hasOpenProject();

    void openProjectFile(QString);

    void applyProjectSettings();

    bool hasDataFile();

    void setOpenWith(QString);

    void setDeleteConfirmed(bool);
    void setDeleteFromDisk(bool);

    bool closeFileWindow(QString);
    void renameFileWindow(QString, QString);





    ~MainWindow();

private slots:
    void resizeDataColumns();
    void resizeTextColumns();
    void resizeHeapColumns();
    void resizeStackColumns();
    void resizeRegsColumns();

    void enableCopyCutDelete(bool);
    void enableUndo(bool);
    void enableRedo(bool);
    void projectExplorerMenuRequested(QPoint);
    void editorWindowMenuRequested(QPoint);
    void consoleMenuRequested(QPoint);

    void on_actionNew_triggered();
    void on_actionAssemble_triggered();
    void on_actionSimulate_triggered();
    void on_actionClose_triggered();
    void on_actionTileset_viewer_triggered();
    void on_actionPalette_Viewer_triggered();
    void on_actionTile_loader_triggered();
    void on_actionSprite_Editor_triggered();
    void on_actionReload_Tiles_Memory_triggered();
    void on_actionAssemble_and_Simulate_triggered();
    void on_actionOpen_Project_triggered();
    void on_actionInput_triggered();

    void on_treeFiles_itemDoubleClicked(QTreeWidgetItem *, int);
    void on_treeFiles_itemExpanded(QTreeWidgetItem *);
    void on_treeFiles_itemCollapsed(QTreeWidgetItem *);

    void on_actionViewProjectManager_triggered();
    void on_actionViewAssemble_triggered();
    void on_actionViewCodeEditor_triggered();
    void on_actionViewMemory_triggered();


    void on_actionOpen_triggered();

    void openTreeItem(QObject *);

    void activeWindowCopy();
    void activeWindowCut();
    void activeWindowPaste();
    void activeWindowUndo();
    void activeWindowRedo();
    void activeWindowSelectAll();
    void activeWindowQuickFind();
    void activeWindowFindAndReplace();
    void activeWindowDelete();

    void refreshActions();
    void refreshEditActions();
    void refreshGraphicsAction();

    void rebuildProjectFile();


    void on_actionNew_Project_triggered();
    void on_actionDefaultLayout_triggered();

    void on_actionSave_triggered();
    void on_actionEnable_Graphics_Engine_triggered();

    void simulationComplete();
    void assemblyComplete();

    void on_btnClearLog_clicked();

    void on_actionMemory_Dump_triggered();

    void on_actionGPU_Memory_Dump_triggered();

    void on_btnExportConsole_clicked();

    void on_btnCopyConsole_clicked();

    void on_btnClearConsole_clicked();

public slots:
    void printToConsole(QString);

    void setMainProjectFile(QString);
    void unsetMainProjectFile();

    void renameTextItem(QString);
    void renameMainTextItem(QString);
    void renameDataItem(QString);
    void renameResItem(QString);

    void removeDataFile(QString);
    void removeTextFile(QString);
    void removeMainTextFile(QString);
    void removeResourceFile(QString);

    void assemblingProgress(int);
    void simulationProgress();

    void appendErrorMessage(QString);

private:
    Ui::MainWindow *ui;
    MemoryLoading* memoryLoading;
    Memory* memory;
    TileSetViewer *tileSetViewer;
    PaletteViewer *paletteViewer;
    TileLoader* tileLoader;
    TileEngine *engine;
    InputManager *inputManager;
    SpriteViewer *spriteViewer;
    QVector<int> mainProcessorRegisters;
    Assembler *assem;
    MemoryDump *memoryDump;
    GpuMemoryDump *gpuMemoryDump;
    RegistersModel *regModel;
    MemoryModel *textModel, *dataModel, *stackModel, *heapModel;
    bool assemblerInitialized;
    QFont editorFont;
    IOConsole *console;
    QTimer *timer;

    QTreeWidget *treeWidget;

    QThread simulationThread;
    //QThread simulationThread;
    QProgressBar *simulationBar;


    //Projecet Configuration
    QString currentProjectString;
    static QString projectPath;
    static QString projectFileName;
    static QString projectTitle;
    static QString projectMainFile;
    static QStringList projectTextFiles;
    static QString projectDataFile;
    static QMap<QString, QString> projectConf;

    QFile projectFile;


    bool parseProjectXML(QFile &, bool);
    void loadProjectTree();
    bool validateProjectFiles(bool);
    bool validateTempProjectFiles(bool);

    bool closeAllWindows();

    void closeEvent(QCloseEvent *);
    void dropEvent(QDropEvent *);
    void dragEnterEvent(QDragEnterEvent *);


    bool deleteConfirmed;
    bool deleteFromDisk;

    QString openWithArg;

    bool assembling;
    bool simulating;
    bool simulateAfterAssembling;
signals:
    void assembleSignal(QStringList, QStringList);
    void simulateSignal();
};

#endif // MAINWINDOW_H
