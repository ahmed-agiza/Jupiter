#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "syntaxhl.h"
#include <QCompleter>
#include "assembler.h"
#include "loadmemorythread.h"

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

    void printS();

private:
    Ui::MainWindow *ui;
    Memory* mem;
    LoadMemoryThread* myThread;
    //Menus
};

#endif // MAINWINDOW_H
