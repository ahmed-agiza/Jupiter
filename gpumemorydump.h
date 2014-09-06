#ifndef GPUMEMORYDUMP_H
#define GPUMEMORYDUMP_H

#include <QDialog>
#include <QStringList>
#include "memory.h"
#include "loadmemorythread.h"
#include "savememorythread.h"
#include "loadtilesetsthread.h"

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

public slots:
    void onNumberChanged(int);
    void complete();
public:
    explicit GpuMemoryDump(QWidget *parent, Memory *memory, MainWindow *);
    ~GpuMemoryDump();
    void fillStdPalette();
private:
    Ui::GpuMemoryDump *ui;
    Memory *mem;
    LoadMemoryThread *loadingThread;
    SaveMemoryThread *savingThread;
    LoadTilesetsThread *loadTilesetsThread;
    MainWindow *mainW;
    QStringList nameList;
    int claculateMemorySize(const QVector<bool>&);
};

#endif // GPUMEMORYDUMP_H
