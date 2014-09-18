#include "logiccalculator.h"
#include "ui_logiccalculator.h"
#include <QDebug>

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

void LogicCalculator::toggleBin(int index)
{
    bool b=1;
    if(index==1){
        uint base = (ui->hexbinlabel_1->text()=="0x")? 16:10;
        uint value = ui->inputLineEdit_1->text().toUInt(&b,base);
        ui->inputLineEdit_1->setText(QString::number(value,2));
        ui->hexbinlabel_1->setText("0b");
    }else if(index == 2){
        uint base = (ui->hexbinlabel_2->text()=="0x")? 16:10;
        uint value = ui->inputLineEdit_2->text().toUInt(&b,base);
        ui->inputLineEdit_2->setText(QString::number(value,2));
        ui->hexbinlabel_2->setText("0b");
    }else{
        uint base = (ui->hexbinlabel_3->text()=="0x")? 16:10;
        uint value = ui->numberDisplay->text().toUInt(&b,base);
        ui->numberDisplay->setText(QString::number(value,2));
        ui->hexbinlabel_3->setText("0b");
    }
}

void LogicCalculator::toggleDec(int index)
{
    bool b=1;
    if(index==1){
        uint base = (ui->hexbinlabel_1->text()=="0x")? 16:2;
        uint value = ui->inputLineEdit_1->text().toUInt(&b,base);
        ui->inputLineEdit_1->setText(QString::number(value,10));
        ui->hexbinlabel_1->setText("");
    }else if(index == 2){
        uint base = (ui->hexbinlabel_2->text()=="0x")? 16:2;
        uint value = ui->inputLineEdit_2->text().toUInt(&b,base);
        ui->inputLineEdit_2->setText(QString::number(value,10));
        ui->hexbinlabel_2->setText("");
    }else{
        uint base = (ui->hexbinlabel_3->text()=="0x")? 16:2;
        uint value = ui->numberDisplay->text().toUInt(&b,base);
        ui->numberDisplay->setText(QString::number(value,10));
        ui->hexbinlabel_3->setText("");
    }
}

void LogicCalculator::toggleHex(int index)
{
    bool b=1;
    if(index==1){
        uint base = (ui->hexbinlabel_1->text()=="0b")? 2:10;
        uint value = ui->inputLineEdit_1->text().toUInt(&b,base);
        ui->inputLineEdit_1->setText(QString::number(value,16));
        ui->hexbinlabel_1->setText("0x");
    }else if(index == 2){
        uint base = (ui->hexbinlabel_2->text()=="0b")? 2:10;
        uint value = ui->inputLineEdit_2->text().toUInt(&b,base);
        ui->inputLineEdit_2->setText(QString::number(value,16));
        ui->hexbinlabel_2->setText("0x");
    }else{
        uint base = (ui->hexbinlabel_3->text()=="0b")? 2:10;
        uint value = ui->numberDisplay->text().toUInt(&b,base);
        ui->numberDisplay->setText(QString::number(value,16));
        ui->hexbinlabel_3->setText("0x");
    }
}

void LogicCalculator::on_inputLineEdit_1_textChanged(const QString &arg1)
{
    QString lbl = ui->hexbinlabel_1->text();
    QString str = ui->inputLineEdit_1->text();
    uint base = (lbl=="0b")? 2:((lbl=="0x")? 16:10);
    if(base == 10){
        if(str.startsWith("0x")){
            str.remove(0,2);
            ui->radioButton_hex_1->setChecked(1);
            toggleHex(1);
            ui->inputLineEdit_1->setText("");
        }else if(str.startsWith("0b")){
            str.remove(0,2);
            ui->radioButton_bin_1->setChecked(1);
            toggleBin(1);
            ui->inputLineEdit_1->setText("");
        }
    }
}

void LogicCalculator::on_inputLineEdit_2_textChanged(const QString &arg1)
{
    QString lbl = ui->hexbinlabel_2->text();
    QString str = ui->inputLineEdit_2->text();
    uint base = (lbl=="0b")? 2:((lbl=="0x")? 16:10);
    if(base == 10){
        if(str.startsWith("0x")){
            str.remove(0,2);
            ui->radioButton_hex_2->setChecked(1);
            toggleHex(2);
            ui->inputLineEdit_2->setText("");
        }else if(str.startsWith("0b")){
            str.remove(0,2);
            ui->radioButton_bin_2->setChecked(1);
            toggleBin(2);
            ui->inputLineEdit_2->setText("");
        }
    }
}





void LogicCalculator::on_radioButton_bin_1_toggled(bool checked)
{
    if(checked){
        toggleBin(1);
    }
}

void LogicCalculator::on_radioButton_dec_1_toggled(bool checked)
{
    if(checked){
        toggleDec(1);
    }
}

void LogicCalculator::on_radioButton_hex_1_toggled(bool checked)
{
    if(checked){
        toggleHex(1);
    }
}

void LogicCalculator::on_radioButton_bin_2_toggled(bool checked)
{
    if(checked){
        toggleBin(2);
    }
}

void LogicCalculator::on_radioButton_dec_2_toggled(bool checked)
{
    if(checked){
        toggleDec(2);
    }
}

void LogicCalculator::on_radioButton_hex_2_toggled(bool checked)
{
    if(checked){
        toggleHex(2);
    }
}

void LogicCalculator::on_radioButton_bin_3_toggled(bool checked)
{
    if(checked){
        toggleBin(3);
    }
}

void LogicCalculator::on_radioButton_dec_3_toggled(bool checked)
{
    if(checked){
        toggleDec(3);
    }
}

void LogicCalculator::on_radioButton_hex_3_toggled(bool checked)
{
    if(checked){
        toggleHex(3);
    }
}

void LogicCalculator::on_button_not_1_clicked()
{
    bool b=1;
    uint base = (ui->hexbinlabel_1->text()=="0b")? 2:((ui->hexbinlabel_1->text()=="0x")? 16:10);
    uint value = ui->inputLineEdit_1->text().toUInt(&b,base);
    value = ~value;
    ui->inputLineEdit_1->setText(QString::number(value,base));
}

void LogicCalculator::on_button_not_2_clicked()
{
    bool b=1;
    uint base = (ui->hexbinlabel_2->text()=="0b")? 2:((ui->hexbinlabel_2->text()=="0x")? 16:10);
    uint value = ui->inputLineEdit_2->text().toUInt(&b,base);
    value = ~value;
    ui->inputLineEdit_2->setText(QString::number(value,base));
}

void LogicCalculator::on_button_neg_1_clicked()
{
    bool b=1;
    uint base = (ui->hexbinlabel_1->text()=="0b")? 2:((ui->hexbinlabel_1->text()=="0x")? 16:10);
    uint value = ui->inputLineEdit_1->text().toUInt(&b,base);
    value = -value;
    ui->inputLineEdit_1->setText(QString::number(value,base));
}

void LogicCalculator::on_button_neg_2_clicked()
{
    bool b=1;
    uint base = (ui->hexbinlabel_2->text()=="0b")? 2:((ui->hexbinlabel_2->text()=="0x")? 16:10);
    uint value = ui->inputLineEdit_2->text().toUInt(&b,base);
    value = -value;
    ui->inputLineEdit_2->setText(QString::number(value,base));
}

void LogicCalculator::on_button_sll_clicked()
{
    bool b=1;
    uint base1 = (ui->hexbinlabel_1->text()=="0b")? 2:((ui->hexbinlabel_1->text()=="0x")? 16:10);
    uint value1 = ui->inputLineEdit_1->text().toUInt(&b,base1);
    uint base2 = (ui->hexbinlabel_2->text()=="0b")? 2:((ui->hexbinlabel_2->text()=="0x")? 16:10);
    uint value2 = ui->inputLineEdit_2->text().toUInt(&b,base2);
    uint base3 = (ui->hexbinlabel_3->text()=="0b")? 2:((ui->hexbinlabel_3->text()=="0x")? 16:10);;
    uint value = value1 << value2;
    ui->numberDisplay->setText(QString::number(value,base3));
}

void LogicCalculator::on_button_srl_clicked()
{
    bool b=1;
    uint base1 = (ui->hexbinlabel_1->text()=="0b")? 2:((ui->hexbinlabel_1->text()=="0x")? 16:10);
    uint value1 = ui->inputLineEdit_1->text().toUInt(&b,base1);
    uint base2 = (ui->hexbinlabel_2->text()=="0b")? 2:((ui->hexbinlabel_2->text()=="0x")? 16:10);
    uint value2 = ui->inputLineEdit_2->text().toUInt(&b,base2);
    uint base3 = (ui->hexbinlabel_3->text()=="0b")? 2:((ui->hexbinlabel_3->text()=="0x")? 16:10);
    uint value = value1 >> value2;
    ui->numberDisplay->setText(QString::number(value,base3));
}

void LogicCalculator::on_button_and_clicked()
{
    bool b=1;
    uint base1 = (ui->hexbinlabel_1->text()=="0b")? 2:((ui->hexbinlabel_1->text()=="0x")? 16:10);
    uint value1 = ui->inputLineEdit_1->text().toUInt(&b,base1);
    uint base2 = (ui->hexbinlabel_2->text()=="0b")? 2:((ui->hexbinlabel_2->text()=="0x")? 16:10);
    uint value2 = ui->inputLineEdit_2->text().toUInt(&b,base2);
    uint base3 = (ui->hexbinlabel_3->text()=="0b")? 2:((ui->hexbinlabel_3->text()=="0x")? 16:10);
    uint value = value1 & value2;
    ui->numberDisplay->setText(QString::number(value,base3));
}

void LogicCalculator::on_button_or_clicked()
{
    bool b=1;
    uint base1 = (ui->hexbinlabel_1->text()=="0b")? 2:((ui->hexbinlabel_1->text()=="0x")? 16:10);
    uint value1 = ui->inputLineEdit_1->text().toUInt(&b,base1);
    uint base2 = (ui->hexbinlabel_2->text()=="0b")? 2:((ui->hexbinlabel_2->text()=="0x")? 16:10);
    uint value2 = ui->inputLineEdit_2->text().toUInt(&b,base2);
    uint base3 = (ui->hexbinlabel_3->text()=="0b")? 2:((ui->hexbinlabel_3->text()=="0x")? 16:10);
    uint value = value1 | value2;
    ui->numberDisplay->setText(QString::number(value,base3));
}

void LogicCalculator::on_button_xor_clicked()
{
    bool b=1;
    uint base1 = (ui->hexbinlabel_1->text()=="0b")? 2:((ui->hexbinlabel_1->text()=="0x")? 16:10);
    uint value1 = ui->inputLineEdit_1->text().toUInt(&b,base1);
    uint base2 = (ui->hexbinlabel_2->text()=="0b")? 2:((ui->hexbinlabel_2->text()=="0x")? 16:10);
    uint value2 = ui->inputLineEdit_2->text().toUInt(&b,base2);
    uint base3 = (ui->hexbinlabel_3->text()=="0b")? 2:((ui->hexbinlabel_3->text()=="0x")? 16:10);
    uint value = value1 ^ value2;
    ui->numberDisplay->setText(QString::number(value,base3));
}
