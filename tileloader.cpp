#include "tileloader.h"
#include "ui_tileloader.h"
#include <QFileDialog>
#include <QString>

bool isLittlEndian()
{
    return true;
}

TileLoader::TileLoader(QWidget *parent, Memory *memory) :
    QDialog(parent),
    ui(new Ui::TileLoader)
{
    this->memory = memory;
    this->setWindowTitle("Tile Loader");
    ui->setupUi(this);
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

TileLoader::~TileLoader()
{
    delete ui;
}

void TileLoader::on_pushButton_clicked()
{
    filePath = QFileDialog::getOpenFileName(this, tr("Open File"),filePath,tr("PNG files (*.png)"));
    ui->lineEdit->setText(filePath);
}

void TileLoader::setMemory(Memory* memory)
{
    this->memory = memory;
}

sf::Uint32 TileLoader::colorToInt(sf::Color color)
{
    sf::Uint32 red = color.r;
    sf::Uint32 green = color.g;
    sf::Uint32 blue = color.b;
    sf::Uint32 alpha = color.a;
    if(isLittlEndian())
        return ((alpha<<24)|(blue<<16)|(green<<8)|red);
    else
        return ((red<<24)|(green<<16)|(blue<<8)|alpha);
}

void TileLoader::on_pushButton_3_clicked()
{
    if(ui->pushButton_3->text() == "Background"){
        ui->pushButton_3->setText("Sprites");
    }else{
        ui->pushButton_3->setText("Background");
    }
}

void TileLoader::on_pushButton_2_clicked()
{
    sf::Image imageToLoad;
    QVector< QVector< sf::Uint8 > > imageMatrix(16,QVector<sf::Uint8>(16, 0));
    imageToLoad.loadFromFile(filePath.toStdString());
    if(imageToLoad.getSize().x != 16 || imageToLoad.getSize().y != 16){
        qDebug() << "Failed to load the tile. Tile dimensions should be 16x16";
        return;
    }
    int tileIndex = ui->spinBox->value();
    int tileBaseAddress = ((ui->pushButton_3->text() == "Background")? memory->backgroundTileSetBaseAddress:memory->spritesTileSetBaseAddress);
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

void TileLoader::on_lineEdit_editingFinished()
{
    filePath = ui->lineEdit->text();
}
