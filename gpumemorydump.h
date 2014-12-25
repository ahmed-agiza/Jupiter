#ifndef GPUMEMORYDUMP_H
#define GPUMEMORYDUMP_H

#include <QDialog>
#include <QStringList>
#include "memory.h"
#include "loadmemorythread.h"
#include "savememorythread.h"
#include "loadtilesetsthread.h"
#include "savetilesetthread.h"
#include "paletteloadingthread.h"
#include "palettesavingthread.h"
#include "tilemaploadingthread.h"
#include "tilemapsavingthread.h"


namespace Ui {
class GpuMemoryDump;
}

class MainWindow;
class GpuMemoryDump : public QDialog
{
    Q_OBJECT
private slots:
    void on_loadFromFilePushButton_clicked();

    void on_saveToFilePushButton_clicked();

    void on_dumpstdpalette_clicked();

    void on_tilesetPushButton_clicked();

    void on_openTilesetViewer_clicked();

    void on_openPaletteViewer_clicked();

    void on_loadTileImages_clicked();

    void on_saveTileImages_clicked();

    void on_loadTileset_clicked();

    void on_saveTileset_clicked();

    void on_savePaletteImage_clicked();

    void on_loadPaletteImage_clicked();

    void on_loadTilemap_clicked();

    void on_saveTilemap_clicked();

    void on_dynamicDump_clicked();

    void on_checkBox_clicked();

    void on_bgTilesetCheckbox_clicked();
    void on_tilemapsCheckbox_clicked();
    void on_spTilesetCheckbox_clicked();
    void on_paletteCheckbox_clicked();
    void on_oamCheckbox_clicked();
public slots:
    void onNumberChanged(int);
    void complete();
public:
    explicit GpuMemoryDump(QWidget *parent, Memory *memory, MainWindow *);
    ~GpuMemoryDump();
    void fillStdPalette();
    void disableButtons();
private:
    Ui::GpuMemoryDump *ui;
    Memory *mem;
    LoadMemoryThread *loadingThread;
    SaveMemoryThread *savingThread;
    LoadTilesetsThread *loadTilesetsThread;
    SaveTilesetThread *saveTilesetThread;
    PaletteLoadingThread *paletteLoadingThread;
    PaletteSavingThread *paletteSavingThread;
    TilemapLoadingThread *tilemapLoadingThread;
    TilemapSavingThread *tilemapSavingThread;
    MainWindow *mainW;
    QStringList nameList;
    int claculateMemorySize(const QVector<bool>&);

    int checkedCount = 0;
};

#endif // GPUMEMORYDUMP_H
