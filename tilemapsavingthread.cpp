#include "tilemapsavingthread.h"
#include <fstream>
#include <string>
#include "mainwindow.h"

TilemapSavingThread::TilemapSavingThread(QObject *parent, Memory *memory, QString fileName) :
    QThread(parent)
{
    this->fileName = fileName;
    this->memory = memory;
}

void TilemapSavingThread::run()
{
    if(fileName.endsWith(".mtmap")){

    }else{
        ofstream outfile(fileName.toStdString().c_str());
        outfile << "[header]\n";
        outfile << "width=" << MainWindow::getTileMapWidth()*32 <<"\n";
        outfile << "height=" << MainWindow::getTileMapHeight()*24 <<"\n";
        outfile << "tilewidth=" << 16 <<"\n";
        outfile << "tileheight=" << 16 <<"\n";
        outfile << "\n[tilesets]\n\n";
        outfile << "[layer]\n";
        outfile << "type=Tile Layer 1\n";
        outfile << "data=\n";
        int addr = memory->tileMapBaseAddress;
        for(int i=0; i<MainWindow::getTileMapHeight()*24; i++){
            for(int j=0; j<MainWindow::getTileMapWidth()*32; j++){
                outfile<< int((unsigned char)(memory->loadByte(addr++))) << ",";
                emit loadingNumberChanged(addr);
            }
            outfile<<"\n";
        }
        outfile<<"\n";
        outfile.close();
        emit loadComplete();
    }
}
