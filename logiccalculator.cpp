#include "logiccalculator.h"
#include "ui_logiccalculator.h"
#include <QDebug>

LogicCalculator::LogicCalculator(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LogicCalculator)
{
    ui->setupUi(this);
    this->setWindowTitle("Converter");

    decRegex = QRegExp("(0[xb0-9])?([0-9]{1,9}|[1-3][0-9]{9}|4([01][0-9]{8}|2([0-8][0-9]{7}|9([0-3][0-9]{6}|4([0-8][0-9]{5}|9([0-5][0-9]{4}|6([0-6][0-9]{3}|7([01][0-9]{2}|2([0-8][0-9]|9[0-5])))))))))?", Qt::CaseInsensitive);
    hexRegex = QRegExp("(0[bd0-9a-f])?([1-9a-f][0-9a-f]{0,7})?", Qt::CaseInsensitive);
    binRegex = QRegExp("(0[dx01])?(1[01]{0,31})?", Qt::CaseInsensitive);

    decRegexValidator = new QRegExpValidator(decRegex);
    binRegexValidator = new QRegExpValidator(binRegex);
    hexRegexValidator = new QRegExpValidator(hexRegex);

    ui->inputLineEdit_2->setValidator(decRegexValidator);
}

LogicCalculator::~LogicCalculator()
{
    delete decRegexValidator;
    delete binRegexValidator;
    delete hexRegexValidator;
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
        ui->inputLineEdit_1->setValidator(binRegexValidator);
    }else if(index == 2){
        uint base = (ui->hexbinlabel_2->text()=="0x")? 16:10;
        uint value = ui->inputLineEdit_2->text().toUInt(&b,base);
        ui->inputLineEdit_2->setText(QString::number(value,2));
        ui->hexbinlabel_2->setText("0b");
        ui->inputLineEdit_2->setValidator(binRegexValidator);
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
        ui->inputLineEdit_1->setValidator(decRegexValidator);
    }else if(index == 2){
        uint base = (ui->hexbinlabel_2->text()=="0x")? 16:2;
        uint value = ui->inputLineEdit_2->text().toUInt(&b,base);
        ui->inputLineEdit_2->setText(QString::number(value,10));
        ui->hexbinlabel_2->setText("");
        ui->inputLineEdit_2->setValidator(decRegexValidator);
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
        ui->inputLineEdit_1->setValidator(hexRegexValidator);
    }else if(index == 2){
        uint base = (ui->hexbinlabel_2->text()=="0b")? 2:10;
        uint value = ui->inputLineEdit_2->text().toUInt(&b,base);
        ui->inputLineEdit_2->setText(QString::number(value,16));
        ui->hexbinlabel_2->setText("0x");
        ui->inputLineEdit_2->setValidator(hexRegexValidator);
    }else{
        uint base = (ui->hexbinlabel_3->text()=="0b")? 2:10;
        uint value = ui->numberDisplay->text().toUInt(&b,base);
        ui->numberDisplay->setText(QString::number(value,16));
        ui->hexbinlabel_3->setText("0x");
    }
}

void LogicCalculator::on_inputLineEdit_1_textChanged(const QString &arg1)
{
    QString str = ui->inputLineEdit_1->text();

        if(str.startsWith("0x")){
            str.remove(0,2);
            ui->radioButton_hex_1->setChecked(1);
            ui->inputLineEdit_1->setText("");
            toggleHex(1);
            ui->inputLineEdit_1->setText(str);
        }else if(str.startsWith("0b")){
            str.remove(0,2);
            ui->radioButton_bin_1->setChecked(1);
            ui->inputLineEdit_1->setText("");
            toggleBin(1);
            ui->inputLineEdit_1->setText(str);
        }else if(str.startsWith("0d")){
            str.remove(0,2);
            ui->radioButton_dec_1->setChecked(1);
            ui->inputLineEdit_1->setText("");
            toggleDec(1);
            ui->inputLineEdit_1->setText(str);
        }else if(str.startsWith("0") && str.size()>1){
            str.remove(QRegExp("^0+"));
            if(str.size()==0)
                str = "0";
            ui->inputLineEdit_1->setText(str);
        }

}

void LogicCalculator::on_inputLineEdit_2_textChanged(const QString &arg1)
{
    QString lbl = ui->hexbinlabel_2->text();
    QString str = ui->inputLineEdit_2->text();
    uint base = (lbl=="0b")? 2:((lbl=="0x")? 16:10);
    if(base == 10 || base == 2){
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
