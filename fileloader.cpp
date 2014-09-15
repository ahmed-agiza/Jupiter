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
    p = parent;
    init();

}

FileLoader::FileLoader(MainWindow *parent, DropFileType type, QString file) :
    QDialog(parent),
    ui(new Ui::FileLoader)
{
    ui->setupUi(this);
    dialogType = DROP_FILE;
    p = parent;
    fileName = file.trimmed();
    fileType = type;
    init();
    if (file.trimmed() != ""){
        if (fileType == DROP_TEXT){
            hide();
            ui->btnText->click();
        }else if (fileType == DROP_DATA){
            hide();
            ui->btnData->click();
        }
    }

}

void FileLoader::setFileName(QString name){
    fileName = name;
}

void FileLoader::setMain(bool value){
    mainFile = value;
}

void FileLoader::setFileType(DropFileType nType){
    fileType = nType;
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

    }else if (dialogType == ADD_FILE || dialogType == DROP_FILE){
        if (dialogType == ADD_FILE)
            fileName = QFileDialog::getOpenFileName(this, "Add Text File", MainWindow::getProjectPath(), "Mirage Text File(*.mtxt)").trimmed();
        QString strippedFileName = QFileInfo(fileName).fileName();
        if (fileName == "")
            return;
        else if (QRegExp("^[A-Za-z0-9_\\@\\$\\.\\s]*$").indexIn(strippedFileName) == -1){
            QMessageBox::critical(this, "Invalid File Name", "The specified file name contains invalid characters");
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

        QFile textFile(fileName);
        if (!textFile.open(QIODevice::ReadOnly)){
            QMessageBox::critical(this, "Error", "Cannot open the specified file");
            return;
        }else{
            textFile.close();

            if (QFileInfo(projectPath).absolutePath() != QFileInfo(fileName).absolutePath()){
                if(QMessageBox::question(this, "Copy New File", "The added file must exist in the project directory, do you want to copy it?") != QMessageBox::Yes){
                    hide();
                    return;
                }
                if (!QFile::copy(fileName, MainWindow::getProjectPath() + strippedFileName)){
                    QMessageBox::critical(this, "Error", "Failed to copy the file to the project directory");
                    hide();
                    return;
                }
            }

            p->addTextFile(strippedFileName);
            if (QMessageBox::question(this, "Set as Main File", "Do you want to set the added file as main file?") == QMessageBox::Yes)
                p->setMainProjectFile(strippedFileName);
        }
    }
    hide();
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

    }else if (dialogType == ADD_FILE || dialogType == DROP_FILE){
        if (dialogType == ADD_FILE)
            fileName = QFileDialog::getOpenFileName(this, "Add Data File", MainWindow::getProjectPath(), "Mirage Data File(*.mdat)").trimmed();
        QString strippedFileName = QFileInfo(fileName).fileName();
        if (fileName == "")
            return;
        else if (QRegExp("^[A-Za-z0-9_\\@\\$\\.\\s]*$").indexIn(strippedFileName) == -1){
            QMessageBox::critical(this, "Invalid File Name", "The specified file name contains invalid characters");
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

        QFile dataFile(fileName);
        if (!dataFile.open(QIODevice::ReadOnly)){
            QMessageBox::critical(this, "Error", "Cannot open the specified file");
            return;
        }else{
            dataFile.close();

            if (QFileInfo(projectPath).absolutePath() != QFileInfo(fileName).absolutePath()){
                if(QMessageBox::question(this, "Copy New File", "The added file must exist in the project directory, do you want to copy it?") != QMessageBox::Yes){
                    hide();
                    return;
                }
                if (!QFile::copy(fileName, MainWindow::getProjectPath() + strippedFileName)){
                    QMessageBox::critical(this, "Error", "Failed to copy the file to the project directory");
                    hide();
                    return;
                }
            }

            p->addDataFile(strippedFileName);
        }
    }
    hide();
}

void FileLoader::on_btnRes_clicked(){
    hide();
}

void FileLoader::init(){
    setModal(true);
    setFixedSize(width(), height());
    mainFile = false;

    QRect screenGeo = QApplication::desktop()->screenGeometry(0);
    QPoint centerPoint = screenGeo.center();
    centerPoint.setX(centerPoint.x() - width()/2);
    centerPoint.setY(centerPoint.y() - height()/2);
    move(centerPoint);

    if(dialogType == CREATE_FILE){
        this->setWindowTitle("Creat New File");
        ui->btnRes->setEnabled(false);
    }else if (dialogType == DROP_FILE){
        setWindowTitle("Select the File Type");
    }else
        setWindowTitle("Add Existing File");
}
