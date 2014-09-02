#include "filenamedialog.h"
#include "ui_filenamedialog.h"

FileNameDialog::FileNameDialog(FileLoader *parent, DialogFileType nType) :
    QDialog(parent),
    ui(new Ui::FileNameDialog){
    ui->setupUi(this);
    //setInputMode(QInputDialog::TextInput);
    type = nType;
    p = parent;
    if (type == DATA_DIALOG){
        setWindowTitle("Data File Name");
        ui->chkMain->setVisible(false);
    }else if (type == DATA_DIALOG){
        setWindowTitle("Text File Name");
        ui->chkMain->setVisible(true);
    }
}

FileNameDialog::~FileNameDialog(){
    delete ui;
}

void FileNameDialog::on_BtnOK_clicked(){
    QString fileName = ui->txtFileName->text().trimmed();
    QRegExp fileValidator("^[A-Za-z0-9_\\@\\$.\\s]*$");
    if (fileValidator.indexIn(fileName) == -1){
        QMessageBox::critical(this, "Invalid File", "The file name contains invalid characters");
        hide();
        return;
    }
    p->setFileName(fileName);
    if (type == TEXT_DIALOG)
        p->setMain(ui->chkMain->isChecked());
    hide();

}

void FileNameDialog::on_btnCancel_clicked(){
    hide();
}
