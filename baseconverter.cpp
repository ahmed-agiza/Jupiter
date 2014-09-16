#include "baseconverter.h"
#include "ui_baseconverter.h"

BaseConverter::BaseConverter(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BaseConverter)
{
    ui->setupUi(this);
}

BaseConverter::~BaseConverter()
{
    delete ui;
}

void BaseConverter::on_radioButton_otherBase_toggled(bool checked)
{
    ui->otherBase_lineEdit->setEnabled(checked);
}
