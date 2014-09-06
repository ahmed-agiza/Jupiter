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
    if(fileName.endsWith(".tmx")){
        ofstream outfile(fileName.toStdString().c_str());
        fileName.remove(".tmx");

        outfile << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
        outfile << "<!DOCTYPE map SYSTEM \"http://mapeditor.org/dtd/1.0/map.dtd\">\n";
        outfile << "<map version=\"1.0\" orientation=\"orthogonal\" width=\"" << MainWindow::getTileMapWidth()*32 << "\" height=\"" << MainWindow::getTileMapHeight()*24 << "\" tilewidth=\"16\" tileheight=\"16\">\n";

        outfile << "<tileset firstgid=\"1\" name=\""<< fileName.toStdString() << "\" tilewidth=\"16\" tileheight=\"16\">\n";
        outfile << "<image source=\""<<fileName.toStdString()<<".png\" width=\"256\" height=\"256\"/>\n";
        outfile << "</tileset>\n";
        outfile << "<layer name=\"Tile Layer 1\" width=\"" << MainWindow::getTileMapWidth()*32  << "\" height=\"" << MainWindow::getTileMapHeight()*24 << "\">\n";
        outfile << "<data>\n";

        int addr = memory->tileMapBaseAddress;
        for(int i=0; i<MainWindow::getTileMapHeight()*24; i++){
            for(int j=0; j<MainWindow::getTileMapWidth()*32; j++){
                outfile<< "   <tile gid=\"" << int((unsigned char)(memory->loadByte(addr++)))+1 << "\"/>\n";
                emit loadingNumberChanged(addr);
            }
            outfile<<"\n";
        }
        outfile << "</data>\n";
        outfile << "</layer>\n";
        outfile << "</map>\n";
        outfile.close();

        sf::Image tilesetimg;
        tilesetimg.create(256,256);
        for(int i=0; i<16; i++){
            for(int j=0; j<16; j++){
                tilesetimg.copy(memory->backgroundTileSet[i*16+j].getImage(),j*16,i*16,IntRect(0,0,16,16));
                emit loadingNumberChanged(i*16+j);
            }
        }
        fileName.append(".png");
        tilesetimg.saveToFile(fileName.toStdString());
        emit loadComplete();
    }else if(fileName.endsWith(".txt")){
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
