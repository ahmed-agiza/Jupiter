#ifndef FILENAMEDIALOG_H
#define FILENAMEDIALOG_H

#include <QDialog>

#include "fileloader.h"

namespace Ui {
class FileNameDialog;
}

enum DialogFileType{DATA_DIALOG, TEXT_DIALOG, UNKOWN_DIALOG};

class FileNameDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FileNameDialog(FileLoader *parent = 0, DialogFileType nType = UNKOWN_DIALOG);
    ~FileNameDialog();

private slots:
    void on_BtnOK_clicked();

    void on_btnCancel_clicked();

private:
    Ui::FileNameDialog *ui;
    DialogFileType type;
    FileLoader *p;
};

#endif // FILENAMEDIALOG_H
