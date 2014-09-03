#ifndef FILELOADER_H
#define FILELOADER_H

#include <QDialog>
#include "mainwindow.h"

namespace Ui {
class FileLoader;
}

enum FileDialogType{ADD_FILE, CREATE_FILE, DROP_FILE};
enum DropFileType {DROP_TEXT, DROP_DATA};

class FileLoader : public QDialog
{
    Q_OBJECT

public:
    explicit FileLoader(MainWindow *parent = 0, FileDialogType type = CREATE_FILE);
    explicit FileLoader(MainWindow *parent = 0, DropFileType type = DROP_TEXT, QString file = "");
    void setFileName(QString name);
    void setMain(bool value);
    void setFileType(DropFileType);
    ~FileLoader();

private slots:
    void on_btnText_clicked();

    void on_btnData_clicked();

    void on_btnRes_clicked();

private:
    Ui::FileLoader *ui;
    FileDialogType dialogType;
    MainWindow *p;
    QString fileName;
    DropFileType fileType;
    bool mainFile;
    void init();
};

#endif // FILELOADER_H
