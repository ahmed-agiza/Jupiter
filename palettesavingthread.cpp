#include "palettesavingthread.h"

PaletteSavingThread::PaletteSavingThread(QObject *parent, Memory *memory, QString fileName) :
    QThread(parent)
{
    this->memory = memory;
    this->fileName = fileName;
}


void PaletteSavingThread::run()
{
    sf::Image outimage;
    outimage.create(512,512);
    for(int i=0; i<512; i++){
        for(int j=0; j<512; j++){
            outimage.setPixel(j,i,memory->palette[((i/32)*16)+(j/32)].getColor());
            emit loadingNumberChanged(((i/32)*16)+(j/32));
        }
    }
    outimage.saveToFile(fileName.toStdString());
    emit loadComplete();
}
