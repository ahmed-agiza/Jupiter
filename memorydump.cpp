#include <QFileDialog>
#include "memorydump.h"
#include "ui_memorydump.h"

MemoryDump::MemoryDump(QWidget *parent, Memory *mem) :
    QDialog(parent),
    ui(new Ui::MemoryDump)
{
    this->mem = mem;
    ui->setupUi(this);
    ui->loadFromFilePushButton->setEnabled(1);
    ui->saveToFilePushButton->setEnabled(1);
    ui->textSegmentCheckBox->setEnabled(1);
    ui->dataSegmentCheckBox->setEnabled(1);
    ui->progressBar->setVisible(0);
    this->setWindowTitle("Memory Dump");
}

MemoryDump::~MemoryDump()
{
    delete ui;
}

int MemoryDump::claculateMemorySize(const QVector<bool>& segments)
{
    int totalSize = 0;
    if(segments[0])
        totalSize += mem->textSegmentPhysicalSize;
    if(segments[1])
        totalSize += mem->dataSegmentPhysicalSize;

    return totalSize;
}

void MemoryDump::onNumberChanged(int number)
{
    ui->progressBar->setValue(number);
}

void MemoryDump::complete()
{
    ui->loadFromFilePushButton->setEnabled(1);
    ui->saveToFilePushButton->setEnabled(1);
    ui->textSegmentCheckBox->setEnabled(1);
    ui->dataSegmentCheckBox->setEnabled(1);
    ui->progressBar->setVisible(0);
}
void MemoryDump::on_loadFromFilePushButton_clicked()
{
    QString filePath = "";
    filePath = QFileDialog::getOpenFileName(this, tr("Open File"),filePath,tr("Binary File (*.bin)"));
    if(filePath.size()){
        QObject::connect(mem, SIGNAL(loadingNumberChanged(int)), this, SLOT(onNumberChanged(int)));
        ui->loadFromFilePushButton->setEnabled(false);
        ui->saveToFilePushButton->setEnabled(false);
        ui->textSegmentCheckBox->setEnabled(false);
        ui->dataSegmentCheckBox->setEnabled(false);
        ui->progressBar->setVisible(true);
        QVector<bool> segmentsToLoad(7,0);
        segmentsToLoad[0] = ui->textSegmentCheckBox->isChecked();
        segmentsToLoad[1] = ui->dataSegmentCheckBox->isChecked();
        ui->progressBar->setMaximum((claculateMemorySize(segmentsToLoad) + 1024 - 1)/1024);
        loadingThread = new LoadMemoryThread(this,segmentsToLoad);
        loadingThread->memory = mem;
        loadingThread->filePath = filePath;
        QObject::connect(loadingThread, SIGNAL(loadComplete()), this, SLOT(complete()));
        loadingThread->start();
    }
}

void MemoryDump::on_saveToFilePushButton_clicked()
{
    QString filePath = "";
    filePath = QFileDialog::getSaveFileName(this, tr("Save File"),filePath,tr("Binary File (*.bin)"));
    if(filePath.size()){
        QObject::connect(mem, SIGNAL(savingNumberChanged(int)), this, SLOT(onNumberChanged(int)));
        ui->loadFromFilePushButton->setEnabled(false);
        ui->saveToFilePushButton->setEnabled(false);
        ui->textSegmentCheckBox->setEnabled(false);
        ui->dataSegmentCheckBox->setEnabled(false);
        ui->progressBar->setVisible(true);
        QVector<bool> segmentsToLoad(7,0);
        segmentsToLoad[0] = ui->textSegmentCheckBox->isChecked();
        segmentsToLoad[1] = ui->dataSegmentCheckBox->isChecked();
        ui->progressBar->setMaximum((claculateMemorySize(segmentsToLoad) + 1024 - 1)/1024);
        savingThread = new SaveMemoryThread(this,segmentsToLoad);
        savingThread->memory = mem;
        savingThread->filePath = filePath;
        QObject::connect(savingThread, SIGNAL(saveComplete()), this, SLOT(complete()));
        savingThread->start();
    }
}
