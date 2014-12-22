#include "initializememorydialog.h"
#include "ui_initializememorydialog.h"

InitializeMemoryDialog::InitializeMemoryDialog(QWidget *parent, Memory *mem) :
    QDialog(parent),
    ui(new Ui::InitializeMemoryDialog)
{
    ui->setupUi(this);
    this->mem = mem;
    this->setWindowTitle("Initializing GPU Memory");
    this->setFocusPolicy(Qt::StrongFocus);
    ui->progressBar->setMaximum((mem->gpuTotalSize + 1024 - 1)/1024);
    t = new InitializeMemoryThread(mem, this);
    QObject::connect(mem, SIGNAL(loadingNumberChanged(int)), this, SLOT(onLoadingNumberChanged(int)));
    QObject::connect(t, SIGNAL(loadComplete()), this, SLOT(loadComplete()));

    t->start();

}

InitializeMemoryDialog::~InitializeMemoryDialog()
{
    delete ui;
}

void InitializeMemoryDialog::onLoadingNumberChanged(int number)
{
    ui->progressBar->setValue(number);
}

void InitializeMemoryDialog::loadComplete()
{
    this->close();
}
