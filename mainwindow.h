#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "syntaxhl.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_textCodeEditor_textChanged();

private:
    Ui::MainWindow *ui;
    SyntaxHL *SHL;

    //Menus
};

#endif // MAINWINDOW_H
