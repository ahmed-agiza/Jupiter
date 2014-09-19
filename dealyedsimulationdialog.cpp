#include <QDebug>
#include "dealyedsimulationdialog.h"
#include "ui_dealyedsimulationdialog.h"

DealyedSimulationDialog::DealyedSimulationDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DealyedSimulationDialog)
{
    ui->setupUi(this);
    ui->lblSpeedVal->setFixedSize(ui->lblSpeedVal->size());
    ui->lblInstrPer->setFixedSize(ui->lblSpeed->size());
    ui->lblSpeedVal->setAlignment(Qt::AlignCenter);
    ui->lblInstrPer->setAlignment(Qt::AlignCenter);
    on_speedSlider_valueChanged(0);

}

int DealyedSimulationDialog::getSpeed(){
    return ui->speedSlider->value() * 100;
}

DealyedSimulationDialog::~DealyedSimulationDialog(){
    delete ui;
}

void DealyedSimulationDialog::on_buttonBox_clicked(QAbstractButton *button){

    if ((QPushButton *)button == ui->buttonBox->button(QDialogButtonBox::Ok)){
        return;
    }else
        hide();
}

void DealyedSimulationDialog::on_speedSlider_valueChanged(int value){
    if (!value){
        ui->lblInstrPer->setText("");
        ui->lblSpeedVal->setText("Realtime Simulation");

    }else{
        ui->lblInstrPer->setText("Instruction per");
        ui->lblSpeedVal->setText(QString::number(value * 100) + " milliseconds");
    }
}
