#include "tilemaploadingthread.h"
#include <QFile>
#include <QRegExp>
#include "mainwindow.h"

TilemapLoadingThread::TilemapLoadingThread(QObject *parent, Memory *memory, QString fileName) :
    QThread(parent)
{
    this->fileName = fileName;
    this->memory = memory;
}

void TilemapLoadingThread::run()
{
    if(fileName.endsWith(".tmx")){

    }else if(fileName.endsWith(".txt")){
        QFile myFile(fileName);
        if (!myFile.open(QIODevice::ReadOnly | QIODevice::Text)){
            qDebug() << "cannot open file";
            emit loadComplete();
            return;
        }

        QString mystr(myFile.readLine());
        if(!mystr.startsWith("[header]")) {
            qDebug() << "header not found";
            emit loadComplete();
            return;
        }
        mystr = myFile.readLine();
        QRegExp widthregex("width=(\\d+)");
        if(widthregex.indexIn(mystr) != 0) {
            qDebug() << "width not found";
            emit loadComplete();
            return;
        }
        int width = widthregex.cap(1).toInt();
        mystr = myFile.readLine();
        QRegExp heightregex("height=(\\d+)");
        if(heightregex.indexIn(mystr) != 0) {
            qDebug() << "height not found";
            emit loadComplete();
            return;
        }
        int height = heightregex.cap(1).toInt();
        if((height != 24 && height != 48 && height != 72 && height != 96) ||
           (width != 32 && width != 64 && width != 96 && width != 128)){
            qDebug() << "dimensions invalid";
            emit loadComplete();
            return;
        }
        if(height/24 != MainWindow::getTileMapHeight() || width/32 != MainWindow::getTileMapWidth()){
            qDebug() << "file dimensions does not match tile map size";
            emit loadComplete();
            return;
        }

        mystr = myFile.readLine();
        if(QRegExp("tilewidth=16").indexIn(mystr) != 0) {
            qDebug() << "tilewidth not found";
            emit loadComplete();
            return;
        }
        mystr = myFile.readLine();
        if(QRegExp("tileheight=16").indexIn(mystr) != 0) {
            qDebug() << "tileheight not found";
            emit loadComplete();
            return;
        }

        for(int i=0; i<6; i++)
            mystr = myFile.readLine();

        int addr = memory->tileMapBaseAddress;
        for(int i=0; i<height; i++) {
            mystr = myFile.readLine();
            mystr.remove("\n");
            QVector<QString> slist = mystr.split(",").toVector();
            for(int i=0; i<slist.size()-1; i++){
                memory->storeByte(addr++, char(slist[i].toInt()));
                emit loadingNumberChanged(addr);
            }
        }
        myFile.close();
        emit loadComplete();
    }
}
