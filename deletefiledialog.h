#ifndef DELETEFILEDIALOG_H
#define DELETEFILEDIALOG_H

#include <QDialog>
#include "mainwindow.h"

namespace Ui {
class DeleteFileDialog;
}

class DeleteFileDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DeleteFileDialog(MainWindow *parent = 0);
    void setText(QString text);
    ~DeleteFileDialog();

private slots:

    void on_btnsDialog_clicked(QAbstractButton *button);

private:
    Ui::DeleteFileDialog *ui;
    MainWindow *p;
};

#endif // DELETEFILEDIALOG_H
