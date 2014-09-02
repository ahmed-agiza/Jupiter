#include "deletefiledialog.h"
#include "ui_deletefiledialog.h"
#include <QAbstractButton>
#include <QDialogButtonBox>

DeleteFileDialog::DeleteFileDialog(MainWindow *parent) :
    QDialog(parent),
    ui(new Ui::DeleteFileDialog)
{
    ui->setupUi(this);
    p = parent;
}

void DeleteFileDialog::setText(QString text){
    ui->lblMessage->setText(text.trimmed());
}

DeleteFileDialog::~DeleteFileDialog()
{
    delete ui;
}



void DeleteFileDialog::on_btnsDialog_clicked(QAbstractButton *button){
    if (ui->btnsDialog->standardButton(button) == QDialogButtonBox::Yes){
        p->setDeleteConfirmed(true);
        p->setDeleteFromDisk(ui->chkDisk->isChecked());
    }
    hide();
}
