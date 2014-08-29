#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "syntaxhl.h"
#include <QCompleter>
#include "assembler.h"
#include "loadmemorythread.h"
#include "memoryloading.h"
#include "tilesetviewer.h"
#include "paletteviewer.h"
#include "tileloader.h"
#include "registersmodel.h"
#include "memorymodel.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    bool eventFilter(QObject *, QEvent *);
    ~MainWindow();

private slots:
    void on_actionNew_triggered();

    void on_actionAssemble_triggered();

    void on_actionSimulate_triggered();

    void on_actionClose_triggered();

    void on_actionTileset_viewer_triggered();

    void on_actionPalette_Viewer_triggered();

    void on_actionTile_loader_triggered();

    void printS();

    void on_actionEnable_Graphics_Engine_triggered();

    void on_actionReload_Tiles_Memory_triggered();

private:
    Ui::MainWindow *ui;
    MemoryLoading* memoryLoading;
    Memory* memory;
    TileSetViewer *tileSetViewer;
    PaletteViewer *paletteViewer;
    TileLoader* tileLoader;
    TileEngine *engine;
    QVector<int> mainProcessorRegisters;
    Assembler *assem;
    RegistersModel *regModel;
    MemoryModel *textModel, *dataModel, *stackModel, *heapModel;
    bool assemblerInitialized;
    //Menus
};

#endif // MAINWINDOW_H
