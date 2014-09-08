#include "loadtilesetsthread.h"
#include <QImage>
#include <QRegExp>
#include <QStringList>
#include "mainwindow.h"

LoadTilesetsThread::LoadTilesetsThread(QObject *parent, Memory* memory, bool tileSetIndex, bool indImages, QString fileName, QStringList *nameList) :
    QThread(parent)
{
    this->memory = memory;
    this->nameList = nameList;
    this->tileSetIndex = tileSetIndex;
    this->indImages = indImages;
    this->fileName = fileName;


    for(int i=0; i<256; i++){
        paletteSet[colorToInt(memory->palette[palettePointer].getColor())] = palettePointer;
    }
}

void LoadTilesetsThread::run()
{
    if(indImages){
        OpenMultiple();
    }else{
        OpenSingle();
    }
}

void LoadTilesetsThread::OpenMultiple()
{
    int palettePointer = 0;
    bool firstBlack=false;
    while(palettePointer<256 && (memory->palette[palettePointer].getColor() != sf::Color(0,0,0,0) || !firstBlack)){
        if(memory->palette[palettePointer].getColor() == sf::Color(0,0,0,0))
            firstBlack = true;
        palettePointer++;
    }
    QRegExp validNumber (QString("((?:2[0-4]\\d)|(?:25[0-5])|(?:[01]?\\d?\\d))"),Qt::CaseInsensitive);
    sf::Image imageToLoad;
    int count = 256-nameList->size();
    emit loadingNumberChanged(count);
    QVector< QVector< sf::Uint8 > > imageMatrix(16,QVector<sf::Uint8>(16, 0));
    foreach(QString filePath, *nameList){
        QString temp = filePath;
        QString fileIndex = temp.remove(QRegExp(".+/")).remove(QRegExp("\\..+"));

        if(validNumber.indexIn(fileIndex) == 0){
            if(imageToLoad.loadFromFile(filePath.toStdString())){
                if(imageToLoad.getSize().x != 16 || imageToLoad.getSize().y != 16){
                    tilesFailed.push_back(fileIndex.toInt());
                }
                int tileIndex = fileIndex.toInt();
                int tileBaseAddress = (!tileSetIndex)? memory->backgroundTileSetBaseAddress:memory->spritesTileSetBaseAddress;
                for(int i=0; i<16; i++){
                    for(int j=0; j<16; j++){
                        Uint32 color = colorToInt(imageToLoad.getPixel(j,i));
                        if(paletteSet.contains(color)){
                            imageMatrix[i][j] = paletteSet[color];
                        }else{
                            if(palettePointer<256){
                                while(palettePointer<256 && memory->palette[palettePointer].getColor() != sf::Color(0,0,0,0)){
                                    palettePointer++;
                                }
                                if(palettePointer == 256){
                                    qDebug() << "Error! Color palette is full";
                                    palettePointer = 0;
                                }
                                memory->storeWord(memory->paletteBaseAddress + (palettePointer*4), color);
                                paletteSet[color] = palettePointer;
                                imageMatrix[i][j] = palettePointer;

                            }else{
                                qDebug() << "Error! Color palette is full";
                                palettePointer = 0;
                                memory->storeWord(memory->paletteBaseAddress + (palettePointer*4), color);
                                paletteSet[color] = palettePointer;
                                imageMatrix[i][j] = palettePointer;
                            }
                        }
                    }
                }
                for(int i=0; i<16; i++){
                    for(int j=0; j<16; j++){
                        memory->storeByte((tileBaseAddress | (tileIndex << 8) | (i<<4) | j), imageMatrix[i][j]);
                    }
                }
            }else if(filePath.endsWith(".mtile")){
                std::ifstream inputFile(filePath.toStdString().c_str(), std::ios::binary);
                if(inputFile.fail()){
                    tilesFailed.push_back(fileIndex.toInt());
                }
                int tileIndex = fileIndex.toInt();
                int tileBaseAddress = (!tileSetIndex)? memory->backgroundTileSetBaseAddress:memory->spritesTileSetBaseAddress;
                qDebug() << tileIndex;
                for(int i=0; i<16; i++){
                    for(int j=0; j<16; j++){
                        char colorR, colorG, colorB, colorA;
                        inputFile.get(colorR);
                        inputFile.get(colorG);
                        inputFile.get(colorB);
                        inputFile.get(colorA);
                        Uint32 color = colorToInt(sf::Color(colorR,colorG,colorB,colorA));
                        if(paletteSet.contains(color)){
                            imageMatrix[i][j] = paletteSet[color];
                        }else{
                            if(palettePointer<256){
                                while(palettePointer<256 && memory->palette[palettePointer].getColor() != sf::Color(0,0,0,0)){
                                    palettePointer++;
                                }
                                if(palettePointer == 256){
                                    qDebug() << "Error! Color palette is full";
                                }else{
                                    memory->storeWord(memory->paletteBaseAddress + (palettePointer*4), color);
                                    paletteSet[color] = palettePointer;
                                    imageMatrix[i][j] = palettePointer;
                                }
                            }else{
                                qDebug() << "Error! Color palette is full";
                            }
                        }
                    }
                }
                for(int i=0; i<16; i++){
                    for(int j=0; j<16; j++){
                        memory->storeByte((tileBaseAddress | (tileIndex << 8) | (i<<4) | j), imageMatrix[i][j]);
                    }
                }
            }else{
                tilesFailed.push_back(fileIndex.toInt());
            }
        }else{
            qDebug() << "Failed to load tile \""+ filePath + "\"";
        }
        emit loadingNumberChanged(++count);
    }
    for(int c=0; c<tilesFailed.size(); c++){
        qDebug() << "failed tile: " << tilesFailed[c];
    }
    emit loadComplete();
}

sf::Uint32 LoadTilesetsThread::colorToInt(sf::Color color)
{
    sf::Uint32 red = color.r;
    sf::Uint32 green = color.g;
    sf::Uint32 blue = color.b;
    sf::Uint32 alpha = color.a;
    if(MainWindow::isLittleEndian())
        return ((alpha<<24)|(blue<<16)|(green<<8)|red);
    else
        return ((red<<24)|(green<<16)|(blue<<8)|alpha);
}

void LoadTilesetsThread::OpenSingle()
{
    sf::Image imageToLoad;
    QVector< QVector< sf::Uint8 > > imageMatrix(16,QVector<sf::Uint8>(16, 0));
    int palettePointer = 0;
    if(imageToLoad.loadFromFile(fileName.toStdString())){
        if(imageToLoad.getSize().x % 16 != 0 || imageToLoad.getSize().y%16 != 0 || (imageToLoad.getSize().x/16)*(imageToLoad.getSize().y/16) != 256){
            qDebug() << "Failed to load tileset \""+ fileName + "\"";
        }else{
            for(int i=0; i<imageToLoad.getSize().y; i+=16){
                for(int j=0; j<imageToLoad.getSize().x; j+=16){
                    sf::Image img;
                    img.create(16,16);
                    for(int r=0; r<16; r++){
                        for(int c=0; c<16; c++){
                            img.setPixel(c,r,imageToLoad.getPixel(j+c,i+r));
                        }
                    }
                    //*****************************************************************
                    int tileIndex = ((i/16)*16+(j/16));

                    int tileBaseAddress = (!tileSetIndex)? memory->backgroundTileSetBaseAddress:memory->spritesTileSetBaseAddress;
                    for(int r=0; r<16; r++){
                        for(int c=0; c<16; c++){
                            Uint32 color = colorToInt(img.getPixel(c,r));
                            if(paletteSet.contains(color)){
                                imageMatrix[r][c] = paletteSet[color];
                            }else{
                                memory->storeWord(memory->paletteBaseAddress + (palettePointer*4), color);
                                paletteSet[color] = palettePointer;
                                imageMatrix[r][c] = palettePointer++;
                            }
                        }
                    }
                    for(int r=0; r<16; r++){
                        for(int c=0; c<16; c++){
                            memory->storeByte((tileBaseAddress | (tileIndex << 8) | (r<<4) | c), imageMatrix[r][c]);
                        }
                    }
                    //*****************************************************************
                    emit loadingNumberChanged(tileIndex);
                }
            }
        }
    }else{
        qDebug() << "Failed to load tileset \""+ fileName + "\"";
    }
    emit loadComplete();
}
