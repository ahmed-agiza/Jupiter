#ifndef INITIALIZEMEMORYDIALOG_H
#define INITIALIZEMEMORYDIALOG_H

#include <QDialog>
#include "memory.h"
#include "initializememorythread.h"
namespace Ui {
class InitializeMemoryDialog;
}

class InitializeMemoryDialog : public QDialog
{
    Q_OBJECT

public:
    explicit InitializeMemoryDialog(QWidget *parent, Memory *mem);
    ~InitializeMemoryDialog();

private:
    Ui::InitializeMemoryDialog *ui;
    Memory* mem;
    InitializeMemoryThread* t;

public slots:
    void loadComplete();
    void onLoadingNumberChanged(int number);
};

#endif // INITIALIZEMEMORYDIALOG_H
