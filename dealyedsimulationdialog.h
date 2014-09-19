#ifndef DEALYEDSIMULATIONDIALOG_H
#define DEALYEDSIMULATIONDIALOG_H

#include <QDialog>
#include <QPushButton>

namespace Ui {
class DealyedSimulationDialog;
}

class DealyedSimulationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DealyedSimulationDialog(QWidget *parent = 0);
    int getSpeed();
    ~DealyedSimulationDialog();

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);

    void on_speedSlider_valueChanged(int value);

private:
    Ui::DealyedSimulationDialog *ui;
};

#endif // DEALYEDSIMULATIONDIALOG_H
