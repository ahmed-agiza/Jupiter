#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "syntaxhl.h"
#include <QCompleter>

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
    void insertCompletion(QString completion);

    void on_textCodeEditor_selectionChanged();

private:
    Ui::MainWindow *ui;
    SyntaxHL *SHL;
    QCompleter *codeCompleter;

    //Menus
};

#endif // MAINWINDOW_H
