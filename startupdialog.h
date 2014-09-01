#ifndef STARTUPDIALOG_H
#define STARTUPDIALOG_H

#include <QDialog>
#include "mainwindow.h"

namespace Ui {
class StartupDialog;
}

class StartupDialog : public QDialog
{
    Q_OBJECT

public:
    explicit StartupDialog(MainWindow *parent = 0);
    ~StartupDialog();

private slots:
    void on_btnNewProject_clicked();

    void on_btnOpenProject_clicked();

private:
    Ui::StartupDialog *ui;
    MainWindow *p;
};

#endif // STARTUPDIALOG_H
