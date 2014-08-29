#include "tilesetviewer.h"
#include "ui_tilesetviewer.h"
#include <QGridLayout>

TileSetViewer::TileSetViewer(QWidget *parent, Memory * memory) :
    QDialog(parent),
    ui(new Ui::TileSetViewer)
{
    ui->setupUi(this);
    this->memory = memory;

    this->setWindowTitle("Tileset Viewer");

    tileRenderWindow = new TileRenderWindow(this,this->memory);
    ui->scrollArea->setWidget(tileRenderWindow);
    connect(this, SIGNAL(renderScreen()), tileRenderWindow, SLOT(repaint()));

    filePath = QDir::currentPath();
    ui->lineEdit->setText(filePath);
    palettePointer = 0;
    bool firstBlack=false;
    for(int i=0; i<256; i++){
        paletteSet[colorToInt(memory->palette[palettePointer].getColor())] = palettePointer;
    }
    while(palettePointer<256 && (memory->palette[palettePointer].getColor() != sf::Color(0,0,0,0) || !firstBlack)){
        if(memory->palette[palettePointer].getColor() == sf::Color(0,0,0,0))
            firstBlack = true;
        palettePointer++;
    }
}

TileSetViewer::~TileSetViewer()
{
    delete ui;
}


void TileSetViewer::on_pushButton_clicked()
{
    if(tileRenderWindow->getTileSetToRender() == 0){
        ui->pushButton->setText("Sprites");
        tileRenderWindow->setTileSetToRender(1);
        emit renderScreen();
    } else {
        ui->pushButton->setText("Background");
        tileRenderWindow->setTileSetToRender(0);
        emit renderScreen();
    }
}

void TileSetViewer::on_horizontalSlider_valueChanged(int value)
{
    tileRenderWindow->setTileIndex(value);
    emit renderScreen();
    ui->label->setText(QString::number(tileRenderWindow->getTileIndex()));
}

void TileSetViewer::on_verticalSlider_valueChanged(int value)
{
    tileRenderWindow->setScaleFactor(value);
    emit renderScreen();
}

bool isLitEndian()
{
    return true;
}

sf::Uint32 TileSetViewer::colorToInt(sf::Color color)
{
    sf::Uint32 red = color.r;
    sf::Uint32 green = color.g;
    sf::Uint32 blue = color.b;
    sf::Uint32 alpha = color.a;
    if(isLitEndian())
        return ((alpha<<24)|(blue<<16)|(green<<8)|red);
    else
        return ((red<<24)|(green<<16)|(blue<<8)|alpha);
}


void TileSetViewer::on_lineEdit_editingFinished()
{
    filePath = ui->lineEdit->text();
}

void TileSetViewer::on_browsePushButton_clicked()
{
    filePath = QFileDialog::getOpenFileName(this, tr("Open File"),filePath,tr("PNG files (*.png)"));
    ui->lineEdit->setText(filePath);
}

void TileSetViewer::on_loadPushButton_clicked()
{
    sf::Image imageToLoad;
    QVector< QVector< sf::Uint8 > > imageMatrix(16,QVector<sf::Uint8>(16, 0));
    qDebug() << filePath;
    if(imageToLoad.loadFromFile(filePath.toStdString())){
        if(imageToLoad.getSize().x != 16 || imageToLoad.getSize().y != 16){
            QImage qImageToLoad;
            QImage qImageAlpha;
            qImageToLoad.load(filePath);
            qImageToLoad.convertToFormat(QImage::Format_ARGB32);
            qImageToLoad = qImageToLoad.scaled(16,16);
            qImageAlpha = qImageToLoad.alphaChannel();
            for(int i=0; i<16; i++){
                for(int j=0; j<16; j++){
                    QColor myQColor(qImageToLoad.pixel(j,i));
                    sf::Color sfmlColor;
                    sfmlColor.r = myQColor.red();
                    sfmlColor.g = myQColor.green();
                    sfmlColor.b = myQColor.blue();
                    sfmlColor.a = qAlpha(qImageAlpha.pixel(j, i));
                    imageToLoad.setPixel(j,i,sfmlColor);
                }
            }
        }
        int tileIndex = ui->horizontalSlider->value();
        int tileBaseAddress = ((ui->pushButton->text() == "Background")? memory->backgroundTileSetBaseAddress:memory->spritesTileSetBaseAddress);
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
    }
    else{
        QMessageBox::information(this, tr("Tile Loader"),
                                 tr("Failed to open tile"),
                                 QMessageBox::Ok);
    }
}

void TileSetViewer::on_savePushButton_clicked()
{
    QString tileNumber = QString::number(ui->horizontalSlider->value());
    if(tileNumber.size() == 1) tileNumber = QString("00") + tileNumber;
    else if(tileNumber.size() == 2) tileNumber = QString("0") + tileNumber;
    QString fileName = "tile" + tileNumber + ".png";
    fileName = QFileDialog::getSaveFileName(this, tr("Save File"),fileName,tr("PNG (*.png)"));
    if(tileRenderWindow->getTileSetToRender() == 0){
        memory->backgroundTileSet[tileRenderWindow->getTileIndex()].getImage().saveToFile(fileName.toStdString());
    }else{
        memory->spritesTileSet[tileRenderWindow->getTileIndex()].getImage().saveToFile(fileName.toStdString());
    }
}
