#include "gpumemorydump.h"
#include "ui_gpumemorydump.h"
#include <QFileDialog>
#include "mainwindow.h"
GpuMemoryDump::GpuMemoryDump(QWidget *parent, Memory *mem, MainWindow *mainW) :
    QDialog(parent),
    ui(new Ui::GpuMemoryDump)
{
    ui->setupUi(this);
    this->mem = mem;
    this->mainW = mainW;
    this->setWindowTitle("GPU Memory Dump");
    complete();
}

GpuMemoryDump::~GpuMemoryDump()
{
    delete ui;
}

void GpuMemoryDump::fillStdPalette()
{
    paletteLoadingThread = new PaletteLoadingThread(this,mem,"",true);
    QObject::connect(paletteLoadingThread, SIGNAL(loadingNumberChanged(int)), this, SLOT(onNumberChanged(int)));
    QObject::connect(paletteLoadingThread, SIGNAL(loadingComplete()), this, SLOT(complete()));
    disableButtons();
    paletteLoadingThread->start();
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
    ui->loadTileImages->setEnabled(1);
    ui->loadTileset->setEnabled(1);
    ui->saveTileImages->setEnabled(1);
    ui->saveTileset->setEnabled(1);
    ui->tilesetPushButton->setEnabled(1);
    ui->openTileEditor->setEnabled(1);
    ui->openTilesetViewer->setEnabled(1);
    ui->dumpstdpalette->setEnabled(1);
    ui->loadPaletteImage->setEnabled(1);
    ui->savePaletteImage->setEnabled(1);
    ui->openPaletteViewer->setEnabled(1);
    ui->openColorEditor->setEnabled(1);
    ui->loadTilemap->setEnabled(1);
    ui->saveTilemap->setEnabled(1);
    ui->progressBar->setVisible(0);
}

void GpuMemoryDump::disableButtons()
{
    ui->loadFromFilePushButton->setDisabled(1);
    ui->saveToFilePushButton->setDisabled(1);
    ui->oamCheckbox->setDisabled(1);
    ui->paletteCheckbox->setDisabled(1);
    ui->tilemapsCheckbox->setDisabled(1);
    ui->bgTilesetCheckbox->setDisabled(1);
    ui->spTilesetCheckbox->setDisabled(1);
    ui->loadTileImages->setDisabled(1);
    ui->loadTileset->setDisabled(1);
    ui->saveTileImages->setDisabled(1);
    ui->saveTileset->setDisabled(1);
    ui->tilesetPushButton->setDisabled(1);
    ui->openTileEditor->setDisabled(1);
    ui->openTilesetViewer->setDisabled(1);
    ui->dumpstdpalette->setDisabled(1);
    ui->loadPaletteImage->setDisabled(1);
    ui->savePaletteImage->setDisabled(1);
    ui->openPaletteViewer->setDisabled(1);
    ui->openColorEditor->setDisabled(1);
    ui->loadTilemap->setDisabled(1);
    ui->saveTilemap->setDisabled(1);
    ui->progressBar->setVisible(1);
}

void GpuMemoryDump::on_loadFromFilePushButton_clicked()
{
    QString filePath = "";
    filePath = QFileDialog::getOpenFileName(this, tr("Open File"),filePath,tr("Binary File (*.bin)"));
    if(filePath.size()){
        QObject::connect(mem, SIGNAL(loadingNumberChanged(int)), this, SLOT(onNumberChanged(int)));
        disableButtons();
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
        disableButtons();
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

void GpuMemoryDump::on_tilesetPushButton_clicked()
{
    if(ui->tilesetPushButton->text() == "Background tileset"){
        ui->tilesetPushButton->setText("Sprites tileset");
    }else{
        ui->tilesetPushButton->setText("Background tileset");
    }
}

void GpuMemoryDump::on_openTilesetViewer_clicked()
{
    mainW->openTilesetViewer();
}

void GpuMemoryDump::on_openPaletteViewer_clicked()
{
    mainW->openPaletteViewer();
}

void GpuMemoryDump::on_loadTileImages_clicked()
{
    QString filePath = "";
    nameList = QFileDialog::getOpenFileNames(this,tr("Open Files"),filePath,tr("Mirage Tile Images(*.mtile);; PNG File(*.png)"));
    if(nameList.size()){
        loadTilesetsThread = new LoadTilesetsThread(this,mem,ui->tilesetPushButton->text().contains("Sprites"),1,filePath,&nameList);
        QObject::connect(loadTilesetsThread, SIGNAL(loadingNumberChanged(int)), this, SLOT(onNumberChanged(int)));
        disableButtons();
        ui->progressBar->setMaximum(256);
        QObject::connect(loadTilesetsThread, SIGNAL(loadComplete()), this, SLOT(complete()));
        loadTilesetsThread->start();
    }
}

void GpuMemoryDump::on_saveTileImages_clicked()
{
    saveTilesetThread = new SaveTilesetThread(this,mem,ui->tilesetPushButton->text().contains("Sprites"),1,"");
    QObject::connect(saveTilesetThread, SIGNAL(loadingNumberChanged(int)), this, SLOT(onNumberChanged(int)));
    disableButtons();
    ui->progressBar->setMaximum(256);
    QObject::connect(saveTilesetThread, SIGNAL(loadComplete()), this, SLOT(complete()));
    saveTilesetThread->start();
}

void GpuMemoryDump::on_loadTileset_clicked()
{
    QString filePath = "";
    filePath = QFileDialog::getOpenFileName(this, tr("Open File"),filePath,tr("PNG File(*.png)"));
    if(filePath.size()){
        loadTilesetsThread = new LoadTilesetsThread(this,mem,ui->tilesetPushButton->text().contains("Sprites"),0,filePath,&nameList);
        QObject::connect(loadTilesetsThread, SIGNAL(loadingNumberChanged(int)), this, SLOT(onNumberChanged(int)));
        disableButtons();
        ui->progressBar->setMaximum(256);
        QObject::connect(loadTilesetsThread, SIGNAL(loadComplete()), this, SLOT(complete()));
        loadTilesetsThread->start();
    }

}

void GpuMemoryDump::on_saveTileset_clicked()
{
    QString filePath = "";
    filePath = QFileDialog::getSaveFileName(this, tr("Open File"),filePath,tr("PNG File(*.png)"));
    if(filePath.size()){
        saveTilesetThread = new SaveTilesetThread(this,mem,ui->tilesetPushButton->text().contains("Sprites"),0,filePath);
        QObject::connect(saveTilesetThread, SIGNAL(loadingNumberChanged(int)), this, SLOT(onNumberChanged(int)));
        disableButtons();
        ui->progressBar->setMaximum(256);
        QObject::connect(saveTilesetThread, SIGNAL(loadComplete()), this, SLOT(complete()));
        saveTilesetThread->start();
    }
}

void GpuMemoryDump::on_savePaletteImage_clicked()
{
    QString filePath = "";
    filePath = QFileDialog::getSaveFileName(this, tr("Save File"),filePath,tr("PNG File(*.png)"));
    if(filePath.size()){
        paletteSavingThread = new PaletteSavingThread(this,mem,filePath);
        QObject::connect(paletteSavingThread, SIGNAL(loadingNumberChanged(int)), this, SLOT(onNumberChanged(int)));
        QObject::connect(paletteSavingThread, SIGNAL(loadComplete()), this, SLOT(complete()));
        disableButtons();
        paletteSavingThread->start();
    }
}

void GpuMemoryDump::on_loadPaletteImage_clicked()
{
    QString filePath = "";
    filePath = QFileDialog::getOpenFileName(this, tr("Open File"),filePath,tr("PNG File(*.png)"));
    if(filePath.size()){
        paletteLoadingThread = new PaletteLoadingThread(this,mem,filePath,false);
        QObject::connect(paletteLoadingThread, SIGNAL(loadingNumberChanged(int)), this, SLOT(onNumberChanged(int)));
        QObject::connect(paletteLoadingThread, SIGNAL(loadingComplete()), this, SLOT(complete()));
        disableButtons();
        paletteLoadingThread->start();
    }
}

void GpuMemoryDump::on_loadTilemap_clicked()
{
    QString filePath = "";
    filePath = QFileDialog::getOpenFileName(this, tr("Open File"),filePath,tr("Tiled map file(*.tmx)"));
    if(filePath.size()){
        tilemapLoadingThread = new TilemapLoadingThread(this,mem,filePath);
        QObject::connect(tilemapLoadingThread, SIGNAL(loadingNumberChanged(int)), this, SLOT(onNumberChanged(int)));
        QObject::connect(tilemapLoadingThread, SIGNAL(loadComplete()), this, SLOT(complete()));
        ui->progressBar->setMaximum(mem->tileMapPhysicalSize);
        disableButtons();
        tilemapLoadingThread->start();
    }
}

void GpuMemoryDump::on_saveTilemap_clicked()
{
    QString filePath = "";
    filePath = QFileDialog::getSaveFileName(this, tr("Save File"),filePath,tr("Tiled map file(*.tmx)"));
    if(filePath.size()){
        tilemapSavingThread = new TilemapSavingThread(this,mem,filePath);
        QObject::connect(tilemapSavingThread, SIGNAL(loadingNumberChanged(int)), this, SLOT(onNumberChanged(int)));
        QObject::connect(tilemapSavingThread, SIGNAL(loadComplete()), this, SLOT(complete()));
        ui->progressBar->setMaximum(mem->tileMapPhysicalSize);
        disableButtons();
        tilemapSavingThread->start();
    }
}
