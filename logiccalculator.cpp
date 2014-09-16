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

}

void LogicCalculator::on_radioButton_dec_1_toggled(bool checked)
{

}

void LogicCalculator::on_radioButton_hex_1_toggled(bool checked)
{

}

void LogicCalculator::on_radioButton_bin_2_toggled(bool checked)
{

}

void LogicCalculator::on_radioButton_dec_2_toggled(bool checked)
{

}

void LogicCalculator::on_radioButton_hex_2_toggled(bool checked)
{

}

void LogicCalculator::on_radioButton_bin_3_toggled(bool checked)
{

}


void LogicCalculator::on_radioButton_dec_3_toggled(bool checked)
{

}

void LogicCalculator::on_radioButton_hex_3_toggled(bool checked)
{

}
