#include "gpumemorydump.h"
#include "ui_gpumemorydump.h"
#include <QFileDialog>

GpuMemoryDump::GpuMemoryDump(QWidget *parent, Memory *mem) :
    QDialog(parent),
    ui(new Ui::GpuMemoryDump)
{
    ui->setupUi(this);
    this->mem = mem;
    ui->loadFromFilePushButton->setEnabled(1);
    ui->saveToFilePushButton->setEnabled(1);
    ui->oamCheckbox->setEnabled(1);
    ui->paletteCheckbox->setEnabled(1);
    ui->tilemapsCheckbox->setEnabled(1);
    ui->bgTilesetCheckbox->setEnabled(1);
    ui->spTilesetCheckbox->setEnabled(1);
    ui->progressBar->setVisible(0);
}

GpuMemoryDump::~GpuMemoryDump()
{
    delete ui;
}

void GpuMemoryDump::fillStdPalette()
{
    mem->palette[0].setColor(sf::Color(0,0,0,0));
    int index = 1;
    for(int r=0; r<=256; r+=64)
        for(int g=0; g<=256; g+=64)
            for(int b=0; b<=256; b+=64)
                mem->palette[index++].setColor(sf::Color((r<256)?r:255,(g<256)?g:255,(b<256)?b:255,255));

    mem->palette[126].setColor(sf::Color(32,32,32,255));
    mem->palette[127].setColor(sf::Color(200,200,200,255));

    mem->palette[128].setColor(sf::Color(255,255,255,32));
    index = 129;
    for(int r=0; r<=256; r+=64)
        for(int g=0; g<=256; g+=64)
            for(int b=0; b<=256; b+=64)
                mem->palette[index++].setColor(sf::Color((r<256)?r:255,(g<256)?g:255,(b<256)?b:255,128));

    mem->palette[254].setColor(sf::Color(255,255,255,64));
    mem->palette[255].setColor(sf::Color(255,255,255,192));

}


int GpuMemoryDump::claculateMemorySize(const QVector<bool>& segments)
{
    int totalSize = 0;
    if(segments[2])
        totalSize += mem->backgroundTileSetPhysicalSize;
    if(segments[3])
        totalSize += mem->spritesTileSetPhysicalSize;
    if(segments[4])
        totalSize += mem->tileMapPhysicalSize;
    if(segments[5])
        totalSize += mem->spriteRamPhysicalSize;
    if(segments[6])
        totalSize += mem->palettePhysicalSize;

    return totalSize;
}

void GpuMemoryDump::onNumberChanged(int number)
{
    ui->progressBar->setValue(number);
}

void GpuMemoryDump::complete()
{
    ui->loadFromFilePushButton->setEnabled(1);
    ui->saveToFilePushButton->setEnabled(1);
    ui->oamCheckbox->setEnabled(1);
    ui->paletteCheckbox->setEnabled(1);
    ui->tilemapsCheckbox->setEnabled(1);
    ui->bgTilesetCheckbox->setEnabled(1);
    ui->spTilesetCheckbox->setEnabled(1);
    ui->progressBar->setVisible(0);
}
void GpuMemoryDump::on_loadFromFilePushButton_clicked()
{
    QString filePath = "";
    filePath = QFileDialog::getOpenFileName(this, tr("Open File"),filePath,tr("Binary File (*.bin)"));
    if(filePath.size()){
        QObject::connect(mem, SIGNAL(loadingNumberChanged(int)), this, SLOT(onNumberChanged(int)));
        ui->loadFromFilePushButton->setEnabled(0);
        ui->saveToFilePushButton->setEnabled(0);
        ui->oamCheckbox->setEnabled(0);
        ui->paletteCheckbox->setEnabled(0);
        ui->tilemapsCheckbox->setEnabled(0);
        ui->bgTilesetCheckbox->setEnabled(0);
        ui->spTilesetCheckbox->setEnabled(0);
        ui->progressBar->setVisible(1);
        QVector<bool> segmentsToLoad(7,0);
        segmentsToLoad[2] = ui->bgTilesetCheckbox->isChecked();
        segmentsToLoad[3] = ui->spTilesetCheckbox->isChecked();
        segmentsToLoad[4] = ui->tilemapsCheckbox->isChecked();
        segmentsToLoad[5] = ui->oamCheckbox->isChecked();
        segmentsToLoad[6] = ui->paletteCheckbox->isChecked();
        ui->progressBar->setMaximum((claculateMemorySize(segmentsToLoad) + 1024 - 1)/1024);
        loadingThread = new LoadMemoryThread(this,segmentsToLoad);
        loadingThread->memory = mem;
        loadingThread->filePath = filePath;
        QObject::connect(loadingThread, SIGNAL(loadComplete()), this, SLOT(complete()));
        loadingThread->start();
    }
}

void GpuMemoryDump::on_saveToFilePushButton_clicked()
{
    QString filePath = "";
    filePath = QFileDialog::getSaveFileName(this, tr("Save File"),filePath,tr("Binary File (*.bin)"));
    if(filePath.size()){
        QObject::connect(mem, SIGNAL(savingNumberChanged(int)), this, SLOT(onNumberChanged(int)));
        ui->loadFromFilePushButton->setEnabled(0);
        ui->saveToFilePushButton->setEnabled(0);
        ui->oamCheckbox->setEnabled(0);
        ui->paletteCheckbox->setEnabled(0);
        ui->tilemapsCheckbox->setEnabled(0);
        ui->bgTilesetCheckbox->setEnabled(0);
        ui->spTilesetCheckbox->setEnabled(0);
        ui->progressBar->setVisible(1);
        QVector<bool> segmentsToLoad(7,0);
        segmentsToLoad[2] = ui->bgTilesetCheckbox->isChecked();
        segmentsToLoad[3] = ui->spTilesetCheckbox->isChecked();
        segmentsToLoad[4] = ui->tilemapsCheckbox->isChecked();
        segmentsToLoad[5] = ui->oamCheckbox->isChecked();
        segmentsToLoad[6] = ui->paletteCheckbox->isChecked();
        ui->progressBar->setMaximum((claculateMemorySize(segmentsToLoad) + 1024 - 1)/1024);
        savingThread = new SaveMemoryThread(this,segmentsToLoad);
        savingThread->memory = mem;
        savingThread->filePath = filePath;
        QObject::connect(savingThread, SIGNAL(saveComplete()), this, SLOT(complete()));
        savingThread->start();
    }
}

void GpuMemoryDump::on_dumpstdpalette_clicked()
{
    fillStdPalette();
}
