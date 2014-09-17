#include "logiccalculator.h"
#include "ui_logiccalculator.h"

LogicCalculator::LogicCalculator(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LogicCalculator)
{
    ui->setupUi(this);
    this->setWindowTitle("Converter");
}

LogicCalculator::~LogicCalculator()
{
    delete ui;
}

void LogicCalculator::on_inputLineEdit_1_textChanged(const QString &arg1)
{

}

void LogicCalculator::on_inputLineEdit_2_textChanged(const QString &arg1)
{

}





void LogicCalculator::on_radioButton_bin_1_toggled(bool checked)
{
    if(checked){
        bool b=1;
        uint base = (ui->hexbinlabel_1->text()=="0x")? 16:10;
        uint value = ui->inputLineEdit_1->text().toUInt(&b,base);
        ui->inputLineEdit_1->setText(QString::number(value,2));
        ui->hexbinlabel_1->setText("0b");
    }
}

void LogicCalculator::on_radioButton_dec_1_toggled(bool checked)
{
    if(checked){
        bool b=1;
        uint base = (ui->hexbinlabel_1->text()=="0x")? 16:2;
        uint value = ui->inputLineEdit_1->text().toUInt(&b,base);
        ui->inputLineEdit_1->setText(QString::number(value,10));
        ui->hexbinlabel_1->setText("");
    }
}

void LogicCalculator::on_radioButton_hex_1_toggled(bool checked)
{
    if(checked){
        bool b=1;
        uint base = (ui->hexbinlabel_1->text()=="0b")? 2:10;
        uint value = ui->inputLineEdit_1->text().toUInt(&b,base);
        ui->inputLineEdit_1->setText(QString::number(value,16));
        ui->hexbinlabel_1->setText("0x");
    }
}

void LogicCalculator::on_radioButton_bin_2_toggled(bool checked)
{
    if(checked){
        bool b=1;
        uint base = (ui->hexbinlabel_2->text()=="0x")? 16:10;
        uint value = ui->inputLineEdit_2->text().toUInt(&b,base);
        ui->inputLineEdit_2->setText(QString::number(value,2));
        ui->hexbinlabel_2->setText("0b");
    }
}

void LogicCalculator::on_radioButton_dec_2_toggled(bool checked)
{
    if(checked){
        bool b=1;
        uint base = (ui->hexbinlabel_2->text()=="0x")? 16:2;
        uint value = ui->inputLineEdit_2->text().toUInt(&b,base);
        ui->inputLineEdit_2->setText(QString::number(value,10));
        ui->hexbinlabel_2->setText("");
    }
}

void LogicCalculator::on_radioButton_hex_2_toggled(bool checked)
{
    if(checked){
        bool b=1;
        uint base = (ui->hexbinlabel_2->text()=="0b")? 2:10;
        uint value = ui->inputLineEdit_2->text().toUInt(&b,base);
        ui->inputLineEdit_2->setText(QString::number(value,16));
        ui->hexbinlabel_2->setText("0x");
    }
}

void LogicCalculator::on_radioButton_bin_3_toggled(bool checked)
{
    if(checked){
        bool b=1;
        uint base = (ui->hexbinlabel_3->text()=="0x")? 16:10;
        uint value = ui->numberDisplay->text().toUInt(&b,base);
        ui->numberDisplay->setText(QString::number(value,2));
        ui->hexbinlabel_3->setText("0b");
    }
}

void LogicCalculator::on_radioButton_dec_3_toggled(bool checked)
{
    if(checked){
        bool b=1;
        uint base = (ui->hexbinlabel_3->text()=="0x")? 16:2;
        uint value = ui->numberDisplay->text().toUInt(&b,base);
        ui->numberDisplay->setText(QString::number(value,10));
        ui->hexbinlabel_3->setText("");
    }
}

void LogicCalculator::on_radioButton_hex_3_toggled(bool checked)
{
    if(checked){
        bool b=1;
        uint base = (ui->hexbinlabel_3->text()=="0b")? 2:10;
        uint value = ui->numberDisplay->text().toUInt(&b,base);
        ui->numberDisplay->setText(QString::number(value,16));
        ui->hexbinlabel_3->setText("0x");
    }
}
