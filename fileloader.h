#ifndef FILELOADER_H
#define FILELOADER_H

#include <QDialog>
#include "mainwindow.h"

namespace Ui {
class FileLoader;
}

enum FileDialogType{AddFile, CreateFile};

class FileLoader : public QDialog
{
    Q_OBJECT

public:
    explicit FileLoader(MainWindow *parent = 0, FileDialogType type = CreateFile);
    ~FileLoader();

private slots:
    void on_btnText_clicked();

    void on_btnData_clicked();

    void on_btnRes_clicked();

private:
    Ui::FileLoader *ui;
    FileDialogType dialogType;
    MainWindow *p;
};

#endif // FILELOADER_H
