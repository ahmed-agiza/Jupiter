#include "fileloader.h"
#include "ui_fileloader.h"
#include <QToolButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QApplication>
#include <QDesktopWidget>
#include <QInputDialog>
#include "filenamedialog.h"

FileLoader::FileLoader(MainWindow *parent, FileDialogType type) :
    QDialog(parent),
    ui(new Ui::FileLoader)
{
    ui->setupUi(this);
    dialogType = type;
    this->setModal(true);
    this->setFixedSize(this->width(), this->height());
    p = parent;
    mainFile = false;

    if(dialogType == CREATE_FILE){
        this->setWindowTitle("Creat New File");
        ui->btnRes->setEnabled(false);
    }else
        setWindowTitle("Add Existing File");



    QRect screenGeo = QApplication::desktop()->screenGeometry(0);
    QPoint centerPoint = screenGeo.center();
    centerPoint.setX(centerPoint.x() - this->width()/2);
    centerPoint.setY(centerPoint.y() - this->height()/2);
    move(centerPoint);

}

void FileLoader::setFileName(QString name){
    fileName = name;
}

void FileLoader::setMain(bool value){
    mainFile = value;
}

FileLoader::~FileLoader()
{
    delete ui;
}

void FileLoader::on_btnText_clicked(){
    if (dialogType == CREATE_FILE){
        FileNameDialog *fileDialog = new FileNameDialog(this, TEXT_DIALOG);
        qDebug() << fileDialog->exec();
        qDebug() << "Passed";
        if (fileName == "")
            return;
        else if (QRegExp("^[A-Za-z0-9_\\@\\$.\\s]*$").indexIn(fileName) == -1){
            QMessageBox::critical(this, "Invalid File Name", "The entered file name contains invalid characters");
            return;
        }
        if (!p->hasOpenProject()){
            QMessageBox::critical(this, "Error", "Cannot detect an active project");
            return;
        }

        QString projectPath = p->getProjectPath().trimmed();
        if (projectPath == ""){
            QMessageBox::critical(this, "Error", "Invalid project directory");
            return;
        }

        QFile newTextFile(projectPath + fileName + ".mtxt");
        if (newTextFile.open(QIODevice::ReadOnly)){
            QMessageBox::critical(this, "Error", "A text file with the same name already exists in the project directory");
            return;
        }

        if (newTextFile.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text)){
            newTextFile.close();
            p->addTextFile(fileName + ".mtxt");
            if(mainFile)
                p->setMainProjectFile(fileName + ".mtxt");
        }else{
            QMessageBox::critical(this, "Error", "Could not add the new text file");
        }

    }
    this->hide();
}

void FileLoader::on_btnData_clicked(){
    if (dialogType == CREATE_FILE){
        FileNameDialog *fileDialog = new FileNameDialog(this, DATA_DIALOG);
        qDebug() << fileDialog->exec();
        qDebug() << "Passed";
        if (fileName == "")
            return;
        else if (QRegExp("^[A-Za-z0-9_\\@\\$.\\s]*$").indexIn(fileName) == -1){
            QMessageBox::critical(this, "Invalid File Name", "The entered file name contains invalid characters");
            return;
        }

        if (!p->hasOpenProject()){
            QMessageBox::critical(this, "Error", "Cannot detect an active project");
            return;
        }

        QString projectPath = p->getProjectPath().trimmed();
        if (projectPath == ""){
            QMessageBox::critical(this, "Error", "Invalid project directory");
            return;
        }

        QFile newTextFile(projectPath + fileName + ".mdat");
        if (newTextFile.open(QIODevice::ReadOnly)){
            QMessageBox::critical(this, "Error", "A file with the same name already exists in the project directory");
            return;
        }

        if (newTextFile.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text)){
            newTextFile.close();
            p->addDataFile(fileName + ".mdat");
        }else{
            QMessageBox::critical(this, "Error", "Could not add the new data file");
        }

    }

    this->hide();
}

void FileLoader::on_btnRes_clicked()
{

}
