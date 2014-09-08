#include "tilemaploadingthread.h"
#include <QFile>
#include <QRegExp>
#include "mainwindow.h"
#include <QDomComment>

TilemapLoadingThread::TilemapLoadingThread(QObject *parent, Memory *memory, QString fileName) :
    QThread(parent)
{
    this->fileName = fileName;
    this->memory = memory;
}

void TilemapLoadingThread::run()
{
    if(fileName.endsWith(".tmx")){
        if (fileName.trimmed() == "")
            return;
        qDebug() << fileName;
        QFile data(fileName);
        if(!data.open( QIODevice::ReadOnly | QIODevice::Text ) ){
            QMessageBox::critical(this, "Error", QString("Failed to open tmx file") + QString("\n ") + data.errorString());
            qDebug() << "Failed to open!";
        }else{

            QDomDocument domDocument;
            if(!domDocument.setContent(&data)){
                QMessageBox::critical(this, "Error", "Invalid file");
                qDebug() << "Cannot set content";
                return;
            }

            data.close();


            QDomNodeList mapNodes = domDocument.elementsByTagName("map");

            if (!mapNodes.isEmpty()){
                qDebug() << "\nMap:";
                QDomElement mapElement = mapNodes.at(0).toElement();
                QDomAttr verNode = mapElement.attributeNode("version");
                QDomAttr oriNode = mapElement.attributeNode("orientation");
                QDomAttr widthNode = mapElement.attributeNode("width");
                QDomAttr heightNode = mapElement.attributeNode("height");
                QDomAttr tileWNode = mapElement.attributeNode("tilewidth");
                QDomAttr tileHNode = mapElement.attributeNode("tileheight");
                qDebug() << "version: " << verNode.value();
                qDebug() << "orientation: "  << oriNode.value();
                qDebug() << "width: "  << widthNode.value();
                qDebug() << "height: "  << heightNode.value();
                qDebug() << "tilewidth: "  << tileWNode.value();
                qDebug() << "tileheight: "  << tileHNode.value();
                QDomNodeList tilesetNodes = mapElement.elementsByTagName("tileset");
                if (!tilesetNodes.isEmpty()){
                    qDebug() << "\nTileset";
                    QDomElement tilesetElement = tilesetNodes.at(0).toElement();
                    QDomAttr firstgidNode = tilesetElement.attributeNode("firstgid");
                    QDomAttr nameNode = tilesetElement.attributeNode("name");
                    QDomAttr tilewidthNode = tilesetElement.attributeNode("tilewidth");
                    QDomAttr tileheightNode = tilesetElement.attributeNode("tileheight");
                    qDebug() << "firstgid: " << firstgidNode.value();
                    qDebug() << "name: " << nameNode.value();
                    qDebug() << "tilewidth: " << tilewidthNode.value();
                    qDebug() << "tileheight: "  << tileheightNode.value();
                    QDomNodeList imageNodes = tilesetElement.elementsByTagName("image");
                    if (!imageNodes.isEmpty()){
                        qDebug() << "\nImage:";
                        QDomElement imageElement = imageNodes.at(0).toElement();
                        QDomAttr sourceNode = imageElement.attributeNode("source");
                        QDomAttr widthNode = imageElement.attributeNode("width");
                        QDomAttr heightNode = imageElement.attributeNode("height");
                        qDebug() << "source: " << sourceNode.value();
                        qDebug() << "width: " << widthNode.value();
                        qDebug() << "height: " << heightNode.value();
                    }

                }
                QDomNodeList layerNodes = mapElement.elementsByTagName("layer");
                if (!layerNodes.isEmpty()){
                    qDebug() << "\nLayer:";
                    QDomElement layerElement = layerNodes.at(0).toElement();
                    QDomAttr nameNode = layerElement.attributeNode("name");
                    QDomAttr widthNode = layerElement.attributeNode("width");
                    QDomAttr heightNode = layerElement.attributeNode("height");
                    qDebug() << "name: " << nameNode.value();
                    qDebug() << "width: " << widthNode.value();
                    qDebug() << "height: " << heightNode.value();
                    QDomNodeList dataNodes = layerElement.elementsByTagName("data");
                    if(!dataNodes.isEmpty()){
                        qDebug() << "\nData";
                        QList<int> dataValues;
                        QDomElement dataElement = dataNodes.at(0).toElement();
                        QDomNode child = dataElement.firstChild();

                        while (!child.isNull()){
                            if (child.toElement().tagName().trimmed() == "tile"){
                                QDomAttr gidValueNode = child.toElement().attributeNode("gid");
                                dataValues.append(gidValueNode.value().toInt());
                                qDebug() << "tile: " << gidValueNode.value();
                            }
                            child = child.nextSibling();
                        }
                    }
                }

            }


          return;
        }
        qDebug() << "Failed to parse";
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
