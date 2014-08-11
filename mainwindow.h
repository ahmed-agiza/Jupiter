#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "syntaxhl.h"
#include <QCompleter>
#include "assembler.h"
#include "loadmemorythread.h"
#include "memoryloading.h"
#include "tilesetviewer.h"
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

    void printS();

private:
    Ui::MainWindow *ui;
    MemoryLoading* memoryLoading;
    Memory* memory;
    TileSetViewer *tileSetViewer;
    QVector<int> mainProcessorRegisters;
    Assembler *assem;
    //Menus
};

#endif // MAINWINDOW_H
