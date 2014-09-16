#ifndef BASECONVERTER_H
#define BASECONVERTER_H

#include <QDialog>

namespace Ui {
class BaseConverter;
}

class BaseConverter : public QDialog
{
    Q_OBJECT

public:
    explicit BaseConverter(QWidget *parent = 0);
    ~BaseConverter();

private slots:
    void on_radioButton_otherBase_toggled(bool checked);

private:
    Ui::BaseConverter *ui;
};

#endif // BASECONVERTER_H
