#include "paletteloadingthread.h"

PaletteLoadingThread::PaletteLoadingThread(QObject *parent, Memory *memory, QString fileName, bool stdPalette, bool dynamic) :
    QThread(parent)
{
    this->memory = memory;
    this->fileName = fileName;
    this->stdPalette = stdPalette;
    this->dynamic = dynamic;
}

void PaletteLoadingThread::run()
{
    if(stdPalette){
        fillStdPalette();
    }else{
        sf::Image paletteImage;
        if(paletteImage.loadFromFile(fileName.toStdString())){
            if(paletteImage.getSize().x != 512 || paletteImage.getSize().y !=512){
                qDebug() << "File dimensions must be 512x512";
            }else{
                for(int i=0; i<16; i++){
                    for(int j=0; j<16; j++){
                        memory->palette[(i*16)+j].setColor(paletteImage.getPixel(j*32,i*32));
                        emit loadingNumberChanged((i*16)+j);
                    }
                }
            }
        }else{
            qDebug() << "Unable to open file";
        }
    }
    emit loadingComplete();
}

void PaletteLoadingThread::fillStdPalette()
{
    memory->palette[0].setColor(sf::Color(0,0,0,0));
    emit loadingNumberChanged(1);
    int index = 1;
    for(int r=0; r<=256; r+=64)
        for(int g=0; g<=256; g+=64)
            for(int b=0; b<=256; b+=64){
                memory->palette[index++].setColor(sf::Color((r<256)?r:255,(g<256)?g:255,(b<256)?b:255,255));
                emit loadingNumberChanged(index);
            }

    memory->palette[126].setColor(sf::Color(32,32,32,255));
    memory->palette[127].setColor(sf::Color(200,200,200,255));

    memory->palette[128].setColor(sf::Color(255,255,255,32));
    index = 129;
    emit loadingNumberChanged(index);
    for(int r=0; r<=256; r+=64)
        for(int g=0; g<=256; g+=64)
            for(int b=0; b<=256; b+=64){
                memory->palette[index++].setColor(sf::Color((r<256)?r:255,(g<256)?g:255,(b<256)?b:255,128));
                emit loadingNumberChanged(index);
            }

    memory->palette[254].setColor(sf::Color(255,255,255,64));
    memory->palette[255].setColor(sf::Color(255,255,255,192));
    emit loadingNumberChanged(255);
    emit loadingComplete();
}
