#ifndef RECONFIGUREPROJECTDIALOG_H
#define RECONFIGUREPROJECTDIALOG_H

#include <QDialog>
#include "mainwindow.h"

namespace Ui {
class ReconfigureProjectDialog;
}

class ReconfigureProjectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ReconfigureProjectDialog(MainWindow *parent = 0, QString = "", bool = false, bool = true, int = 4, int = 4);
    void setParams(QString = "", bool = false, bool = true, int = 4, int = 4);
    QString getTitle() const;
    bool getLEndianState() const;
    bool getGFXState() const;
    int getTileWidth() const;
    int getTileHeight() const;
    ~ReconfigureProjectDialog();
private slots:
    void enableGFXWidgets();


    void on_btnCancel_clicked();

    void on_btnSave_clicked();

private:
    Ui::ReconfigureProjectDialog *ui;
    MainWindow *p;
};

#endif // RECONFIGUREPROJECTDIALOG_H
