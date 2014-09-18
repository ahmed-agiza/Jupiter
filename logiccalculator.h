#ifndef LOGICCALCULATOR_H
#define LOGICCALCULATOR_H

#include <QDialog>

namespace Ui {
class LogicCalculator;
}

class LogicCalculator : public QDialog
{
    Q_OBJECT

public:
    explicit LogicCalculator(QWidget *parent = 0);
    ~LogicCalculator();

    void toggleBin(int);
    void toggleDec(int);
    void toggleHex(int);

private slots:
    void on_inputLineEdit_1_textChanged(const QString &arg1);

    void on_inputLineEdit_2_textChanged(const QString &arg1);

    void on_radioButton_bin_1_toggled(bool checked);

    void on_radioButton_dec_1_toggled(bool checked);

    void on_radioButton_hex_1_toggled(bool checked);

    void on_radioButton_bin_2_toggled(bool checked);

    void on_radioButton_dec_2_toggled(bool checked);

    void on_radioButton_hex_2_toggled(bool checked);

    void on_radioButton_bin_3_toggled(bool checked);

    void on_radioButton_dec_3_toggled(bool checked);

    void on_radioButton_hex_3_toggled(bool checked);

    void on_button_not_1_clicked();

    void on_button_not_2_clicked();

    void on_button_neg_1_clicked();

    void on_button_neg_2_clicked();

    void on_button_sll_clicked();

    void on_button_srl_clicked();

    void on_button_and_clicked();

    void on_button_or_clicked();

    void on_button_xor_clicked();

private:
    Ui::LogicCalculator *ui;
};

#endif // LOGICCALCULATOR_H
