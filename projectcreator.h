#ifndef PROJECTCREATOR_H
#define PROJECTCREATOR_H

#include <QDialog>
#include "mainwindow.h"

namespace Ui {
class ProjectCreator;
}

class ProjectCreator : public QDialog
{
    Q_OBJECT

public:
    explicit ProjectCreator(MainWindow *parent = 0);
    ~ProjectCreator();

private slots:
    void enableGFXWidgets();

    void on_btnBrowse_clicked();

    void on_btnCreate_clicked();

private:
    void buildProject(QString projectName);
    MainWindow *p;
    Ui::ProjectCreator *ui;
};

#endif // PROJECTCREATOR_H
