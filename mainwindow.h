#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeView>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QCompleter>
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

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    bool eventFilter(QObject *, QEvent *);

    static QString getProjectPath();
    static QString getProjectTitle();
    static QString getProjectMainFile();
    static QStringList getProjectTextFiles();
    static QString getProjectDataFile();
    static QString getProjectConf(QString key);
    static bool isLittleEndian();
    static bool isGFXEnabled();
    static int getTileMapWidth();
    static int getTileMapHeight();



    ~MainWindow();

private slots:
    void on_actionNew_triggered();

    void on_actionAssemble_triggered();

    void on_actionSimulate_triggered();

    void on_actionClose_triggered();

    void on_actionTileset_viewer_triggered();

    void on_actionPalette_Viewer_triggered();

    void on_actionTile_loader_triggered();

    void on_actionSprite_Editor_triggered();

    void printS();

    void on_actionEnable_Graphics_Engine_triggered();

    void on_actionReload_Tiles_Memory_triggered();

    void on_actionAssemble_and_Simulate_triggered();

    void on_actionOpen_Project_triggered();

    void on_actionInput_triggered();

    void on_treeFiles_itemDoubleClicked(QTreeWidgetItem *item, int column);

    void on_treeFiles_itemExpanded(QTreeWidgetItem *item);

    void on_treeFiles_itemCollapsed(QTreeWidgetItem *item);

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
    RegistersModel *regModel;
    MemoryModel *textModel, *dataModel, *stackModel, *heapModel;
    bool assemblerInitialized;
    QFont editorFont;

    QTreeWidget *treeWidget;

    //Projecet Configuration
    QString currentProjectString;
    static QString projectPath;
    static QString projectTitle;
    static QString projectMainFile;
    static QStringList projectTextFiles;
    static QString projectDataFile;
    static QMap<QString, QString> projectConf;


    void parseProjectXML(QFile &);
    void loadProjectTree();
    bool validateProjectFiles(bool forceAll);
    //Menus
};

#endif // MAINWINDOW_H
