/*
void Memory::loadMemory(QString fileName,  QVector<bool> segmentsToLoad, bool dynamic)
{
    QStringList dynamicOutFileList;
    Uint32 loadedNumber = 0;
    Uint32 previousLoadedNumber = 0;
    Uint32 consecutiveValues = 0;
    Uint32 loopsCount = 0;

    int count = 0;
    QFile in(fileName);
    in.open(QIODevice::ReadOnly);

    qDebug() << claculateLoadSize(segmentsToLoad);
    if(claculateLoadSize(segmentsToLoad) != in.size())
        return;
    char* byte = new char;

    if(segmentsToLoad[0]){
        for(int i=0; i<textSegmentPhysicalSize; i++){
            in.getChar(byte);
            storeByte(textSegmentBaseAddress + i, *byte);
            if(count %1024 == 0)
                emit loadingNumberChanged(count / 1024);
            count++;
        }
    }
    qDebug() << count;
    if(segmentsToLoad[1]){
        for(int i=0; i<dataSegmentPhysicalSize; i++){
            in.getChar(byte);
            storeByte(dataSegmentBaseAddress + i, *byte);
            if(count %1024 == 0)
                emit loadingNumberChanged(count / 1024);
            count++;
        }
    }
    qDebug() << count;

    if(segmentsToLoad[2]){
        loadedNumber = previousLoadedNumber = consecutiveValues = 0;
        int byteNumber = 0;
        uint liBaseAddress = this->backgroundTileSetBaseAddress;
        if(dynamic){
            dynamicOutFileList.append("# Dumping background tileset");
            dynamicOutFileList.append(QString("\tli\t$t0, 0x")+QString::number(liBaseAddress,16)+QString("\t# background tileset base address"));
            dynamicOutFileList.append("");
        }

        for(int i=0; i<backgroundTileSetPhysicalSize; i++){
            *byte = 1;
            in.getChar(byte);
            if(dynamic){
                if(MainWindow::isLittleEndian()){
                    loadedNumber |= (uint((unsigned char)(*byte))<<((byteNumber%4)*8));
                }else{
                    loadedNumber |= (uint((unsigned char)(*byte))<<((3-(byteNumber%4))*8));
                }if(byteNumber%4 == 3){
                    if(loadedNumber != previousLoadedNumber){
                        if(consecutiveValues > 6){
                            Uint32 startAddress = (byteNumber-3)-(consecutiveValues * 4)+liBaseAddress;
                            dynamicOutFileList.append("");
                            dynamicOutFileList.append(QString("\taddiu\t$t6, $zero, 0x")+QString::number(startAddress,16));
                            dynamicOutFileList.append(QString("loop") + QString::number(loopsCount) + QString(":") + QString("\tslti\t$t5, $t6, 0x")+QString::number(startAddress + (consecutiveValues * 4),16));
                            dynamicOutFileList.append(QString("\tbeq\t$t5, $zero, exit") + QString::number(loopsCount));
                            if(previousLoadedNumber == 0)
                                dynamicOutFileList.append(QString("\tsw\t$zero, 0($t6)"));
                            else
                                dynamicOutFileList.append(QString("\tsw\t$t1, 0($t6)"));
                            dynamicOutFileList.append(QString("\taddiu\t$t6, $t6, 4"));
                            dynamicOutFileList.append(QString("\tj\tloop") + QString::number(loopsCount));
                            dynamicOutFileList.append(QString("exit") + QString::number(loopsCount) + ":");
                            dynamicOutFileList.append("");
                            loopsCount++;
                        }else{
                            Uint32 startOffset = (byteNumber-3)-(consecutiveValues * 4);
                            for(int i=0; i<consecutiveValues+1; i++){
                                dynamicOutFileList.append(QString("\tsw\t$t1, ")+QString::number(startOffset + i*4,10)+QString("($t0)"));
                            }
                        }
                        if(loadedNumber != 0)
                            dynamicOutFileList.append(QString("\tli\t$t1, 0x")+QString::number(loadedNumber,16));
                        previousLoadedNumber = loadedNumber;
                        consecutiveValues = 0;
                    }else{
                        consecutiveValues++;
                    }
                    loadedNumber = 0;
                }
                byteNumber++;

            }else{
                storeByte(backgroundTileSetBaseAddress + i, *byte);
            }if(count %1024 == 0)
                emit loadingNumberChanged(count / 1024);
            count++;
        }

        if(dynamic){
            if(consecutiveValues > 0){
                if(consecutiveValues > 6){
                    Uint32 startAddress = (byteNumber)-(consecutiveValues * 4)+liBaseAddress;
                    dynamicOutFileList.append("");
                    dynamicOutFileList.append(QString("\taddiu\t$t6, $zero, 0x")+QString::number(startAddress,16));
                    dynamicOutFileList.append(QString("loop") + QString::number(loopsCount) + QString(":") + QString("\tslti\t$t5, $t6, 0x")+QString::number(startAddress + (consecutiveValues * 4),16));
                    dynamicOutFileList.append(QString("\tbeq\t$t5, $zero, exit") + QString::number(loopsCount));
                    if(previousLoadedNumber == 0)
                        dynamicOutFileList.append(QString("\tsw\t$zero, 0($t6)"));
                    else
                        dynamicOutFileList.append(QString("\tsw\t$t1, 0($t6)"));
                    dynamicOutFileList.append(QString("\taddiu\t$t6, $t6, 4"));
                    dynamicOutFileList.append(QString("\tj\tloop") + QString::number(loopsCount));
                    dynamicOutFileList.append(QString("exit") + QString::number(loopsCount) + ":");
                    dynamicOutFileList.append("");
                    loopsCount++;
                }else{
                    Uint32 startOffset = (byteNumber-3)-(consecutiveValues * 4);
                    for(int i=0; i<consecutiveValues+1; i++){
                        dynamicOutFileList.append(QString("\tsw\t$t1, ")+QString::number(startOffset + i*4,10)+QString("($t0)"));
                    }
                }
            }
        }
    }

    qDebug() << count;

    if(segmentsToLoad[3]){
        loadedNumber = previousLoadedNumber = consecutiveValues = 0;
        int byteNumber = 0;
        uint liBaseAddress = this->spritesTileSetBaseAddress;
        if(dynamic){
            dynamicOutFileList.append("# Dumping sprites tileset");
            dynamicOutFileList.append(QString("\tli\t$t0, 0x")+QString::number(liBaseAddress,16)+QString("\t# sprites tileset base address"));
            dynamicOutFileList.append("");
        }

        for(int i=0; i<spritesTileSetPhysicalSize; i++){
            *byte = 1;
            in.getChar(byte);
            if(dynamic){
                if(MainWindow::isLittleEndian()){
                    loadedNumber |= (uint((unsigned char)(*byte))<<((byteNumber%4)*8));
                }else{
                    loadedNumber |= (uint((unsigned char)(*byte))<<((3-(byteNumber%4))*8));
                }if(byteNumber%4 == 3){
                    if(loadedNumber != previousLoadedNumber){
                        if(consecutiveValues > 6){
                            Uint32 startAddress = (byteNumber-3)-(consecutiveValues * 4)+liBaseAddress;
                            dynamicOutFileList.append("");
                            dynamicOutFileList.append(QString("\taddiu\t$t6, $zero, 0x")+QString::number(startAddress,16));
                            dynamicOutFileList.append(QString("loop") + QString::number(loopsCount) + QString(":") + QString("\tslti\t$t5, $t6, 0x")+QString::number(startAddress + (consecutiveValues * 4),16));
                            dynamicOutFileList.append(QString("\tbeq\t$t5, $zero, exit") + QString::number(loopsCount));
                            if(previousLoadedNumber == 0)
                                dynamicOutFileList.append(QString("\tsw\t$zero, 0($t6)"));
                            else
                                dynamicOutFileList.append(QString("\tsw\t$t1, 0($t6)"));
                            dynamicOutFileList.append(QString("\taddiu\t$t6, $t6, 4"));
                            dynamicOutFileList.append(QString("\tj\tloop") + QString::number(loopsCount));
                            dynamicOutFileList.append(QString("exit") + QString::number(loopsCount) + ":");
                            dynamicOutFileList.append("");
                            loopsCount++;
                        }else{
                            Uint32 startOffset = (byteNumber-3)-(consecutiveValues * 4);
                            for(int i=0; i<consecutiveValues+1; i++){
                                dynamicOutFileList.append(QString("\tsw\t$t1, ")+QString::number(startOffset + i*4,10)+QString("($t0)"));
                            }
                        }
                        if(loadedNumber != 0)
                            dynamicOutFileList.append(QString("\tli\t$t1, 0x")+QString::number(loadedNumber,16));
                        previousLoadedNumber = loadedNumber;
                        consecutiveValues = 0;
                    }else{
                        consecutiveValues++;
                    }
                    loadedNumber = 0;
                }
                byteNumber++;

            }

                if(dynamic){
                    if(MainWindow::isLittleEndian()){
                        loadedNumber |= (uint((unsigned char)(*byte))<<((byteNumber%4)*8));
                    }else{
                        loadedNumber |= (uint((unsigned char)(*byte))<<((3-(byteNumber%4))*8));
                    }if(byteNumber%4 == 3){
                        if(loadedNumber == 0){
                            dynamicOutFileList.append(QString("\tsw\t$zero, ")+QString::number(byteNumber-3,10)+QString("($t0)"));
                        }else{
                            if(loadedNumber != previousLoadedNumber){
                                dynamicOutFileList.append(QString("\tli\t$t1, 0x")+QString::number(loadedNumber,16));
                                previousLoadedNumber = loadedNumber;
                            }
                            dynamicOutFileList.append(QString("\tsw\t$t1, ")+QString::number(byteNumber-3,10)+QString("($t0)"));
                            loadedNumber = 0;
                        }
                    }
                    byteNumber++;

                }
                // Comment END
            else{
                storeByte(spritesTileSetBaseAddress + i, *byte);
            }if(count %1024 == 0)
                emit loadingNumberChanged(count / 1024);
            count++;
        }

        if(dynamic){
            if(consecutiveValues > 0){
                if(consecutiveValues > 6){
                    Uint32 startAddress = (byteNumber)-(consecutiveValues * 4)+liBaseAddress;
                    dynamicOutFileList.append("");
                    dynamicOutFileList.append(QString("\taddiu\t$t6, $zero, 0x")+QString::number(startAddress,16));
                    dynamicOutFileList.append(QString("loop") + QString::number(loopsCount) + QString(":") + QString("\tslti\t$t5, $t6, 0x")+QString::number(startAddress + (consecutiveValues * 4),16));
                    dynamicOutFileList.append(QString("\tbeq\t$t5, $zero, exit") + QString::number(loopsCount));
                    if(previousLoadedNumber == 0)
                        dynamicOutFileList.append(QString("\tsw\t$zero, 0($t6)"));
                    else
                        dynamicOutFileList.append(QString("\tsw\t$t1, 0($t6)"));
                    dynamicOutFileList.append(QString("\taddiu\t$t6, $t6, 4"));
                    dynamicOutFileList.append(QString("\tj\tloop") + QString::number(loopsCount));
                    dynamicOutFileList.append(QString("exit") + QString::number(loopsCount) + ":");
                    dynamicOutFileList.append("");
                    loopsCount++;
                }else{
                    Uint32 startOffset = (byteNumber-3)-(consecutiveValues * 4);
                    for(int i=0; i<consecutiveValues+1; i++){
                        dynamicOutFileList.append(QString("\tsw\t$t1, ")+QString::number(startOffset + i*4,10)+QString("($t0)"));
                    }
                }
            }
        }
    }
    qDebug() << count;

    if(segmentsToLoad[4]){

        loadedNumber = previousLoadedNumber = consecutiveValues = 0;
        int byteNumber = 0;
        uint liBaseAddress = this->tileMapBaseAddress;
        if(dynamic){
            dynamicOutFileList.append("# Dumping tile maps");
            dynamicOutFileList.append(QString("\tli\t$t0, 0x")+QString::number(liBaseAddress,16)+QString("\t# tile maps base address"));
            dynamicOutFileList.append("");
        }

        for(int i=0; i<tileMapPhysicalSize; i++){
            *byte = 1;
            in.getChar(byte);
            if(dynamic){
                if(MainWindow::isLittleEndian()){
                    loadedNumber |= (uint((unsigned char)(*byte))<<((byteNumber%4)*8));
                }else{
                    loadedNumber |= (uint((unsigned char)(*byte))<<((3-(byteNumber%4))*8));
                }if(byteNumber%4 == 3){
                    if(loadedNumber != previousLoadedNumber){
                        if(consecutiveValues > 6){
                            Uint32 startAddress = (byteNumber-3)-(consecutiveValues * 4)+liBaseAddress;
                            dynamicOutFileList.append("");
                            dynamicOutFileList.append(QString("\taddiu\t$t6, $zero, 0x")+QString::number(startAddress,16));
                            dynamicOutFileList.append(QString("loop") + QString::number(loopsCount) + QString(":") + QString("\tslti\t$t5, $t6, 0x")+QString::number(startAddress + (consecutiveValues * 4),16));
                            dynamicOutFileList.append(QString("\tbeq\t$t5, $zero, exit") + QString::number(loopsCount));
                            if(previousLoadedNumber == 0)
                                dynamicOutFileList.append(QString("\tsw\t$zero, 0($t6)"));
                            else
                                dynamicOutFileList.append(QString("\tsw\t$t1, 0($t6)"));
                            dynamicOutFileList.append(QString("\taddiu\t$t6, $t6, 4"));
                            dynamicOutFileList.append(QString("\tj\tloop") + QString::number(loopsCount));
                            dynamicOutFileList.append(QString("exit") + QString::number(loopsCount) + ":");
                            dynamicOutFileList.append("");
                            loopsCount++;
                        }else{
                            Uint32 startOffset = (byteNumber-3)-(consecutiveValues * 4);
                            for(int i=0; i<consecutiveValues+1; i++){
                                dynamicOutFileList.append(QString("\tsw\t$t1, ")+QString::number(startOffset + i*4,10)+QString("($t0)"));
                            }
                        }
                        if(loadedNumber != 0)
                            dynamicOutFileList.append(QString("\tli\t$t1, 0x")+QString::number(loadedNumber,16));
                        previousLoadedNumber = loadedNumber;
                        consecutiveValues = 0;
                    }else{
                        consecutiveValues++;
                    }
                    loadedNumber = 0;
                }
                byteNumber++;

            }else{
                storeByte(tileMapBaseAddress + i, *byte);
            }if(count %1024 == 0)
                emit loadingNumberChanged(count / 1024);
            count++;
        }

        if(dynamic){
            if(consecutiveValues > 0){
                if(consecutiveValues > 6){
                    Uint32 startAddress = (byteNumber)-(consecutiveValues * 4)+liBaseAddress;
                    dynamicOutFileList.append("");
                    dynamicOutFileList.append(QString("\taddiu\t$t6, $zero, 0x")+QString::number(startAddress,16));
                    dynamicOutFileList.append(QString("loop") + QString::number(loopsCount) + QString(":") + QString("\tslti\t$t5, $t6, 0x")+QString::number(startAddress + (consecutiveValues * 4),16));
                    dynamicOutFileList.append(QString("\tbeq\t$t5, $zero, exit") + QString::number(loopsCount));
                    if(previousLoadedNumber == 0)
                        dynamicOutFileList.append(QString("\tsw\t$zero, 0($t6)"));
                    else
                        dynamicOutFileList.append(QString("\tsw\t$t1, 0($t6)"));
                    dynamicOutFileList.append(QString("\taddiu\t$t6, $t6, 4"));
                    dynamicOutFileList.append(QString("\tj\tloop") + QString::number(loopsCount));
                    dynamicOutFileList.append(QString("exit") + QString::number(loopsCount) + ":");
                    dynamicOutFileList.append("");
                    loopsCount++;
                }else{
                    Uint32 startOffset = (byteNumber-3)-(consecutiveValues * 4);
                    for(int i=0; i<consecutiveValues+1; i++){
                        dynamicOutFileList.append(QString("\tsw\t$t1, ")+QString::number(startOffset + i*4,10)+QString("($t0)"));
                    }
                }
            }
        }
    }
    qDebug() << count;

    if(segmentsToLoad[5]){
        int byteNumber = 0;
        loadedNumber = previousLoadedNumber = consecutiveValues = 0;
        uint liBaseAddress = this->spriteRamBaseAddress;
        if(dynamic){
            dynamicOutFileList.append("# Dumping OAM");
            dynamicOutFileList.append(QString("\tli\t$t0, 0x")+QString::number(liBaseAddress,16)+QString("\t# OAM base address"));
            dynamicOutFileList.append("");
        }
        for(int i=0; i<spriteRamPhysicalSize; i++){
            *byte = 1;
            in.getChar(byte);
            if(dynamic){
                if(MainWindow::isLittleEndian()){
                    loadedNumber |= (uint((unsigned char)(*byte))<<((byteNumber%4)*8));
                }else{
                    loadedNumber |= (uint((unsigned char)(*byte))<<((3-(byteNumber%4))*8));
                }if(byteNumber%4 == 3){
                    if(loadedNumber != previousLoadedNumber){
                        if(consecutiveValues > 6){
                            Uint32 startAddress = (byteNumber-3)-(consecutiveValues * 4)+liBaseAddress;
                            dynamicOutFileList.append("");
                            dynamicOutFileList.append(QString("\taddiu\t$t6, $zero, 0x")+QString::number(startAddress,16));
                            dynamicOutFileList.append(QString("loop") + QString::number(loopsCount) + QString(":") + QString("\tslti\t$t5, $t6, 0x")+QString::number(startAddress + (consecutiveValues * 4),16));
                            dynamicOutFileList.append(QString("\tbeq\t$t5, $zero, exit") + QString::number(loopsCount));
                            if(previousLoadedNumber == 0)
                                dynamicOutFileList.append(QString("\tsw\t$zero, 0($t6)"));
                            else
                                dynamicOutFileList.append(QString("\tsw\t$t1, 0($t6)"));
                            dynamicOutFileList.append(QString("\taddiu\t$t6, $t6, 4"));
                            dynamicOutFileList.append(QString("\tj\tloop") + QString::number(loopsCount));
                            dynamicOutFileList.append(QString("exit") + QString::number(loopsCount) + ":");
                            dynamicOutFileList.append("");
                            loopsCount++;
                        }else{
                            Uint32 startOffset = (byteNumber-3)-(consecutiveValues * 4);
                            for(int i=0; i<consecutiveValues+1; i++){
                                dynamicOutFileList.append(QString("\tsw\t$t1, ")+QString::number(startOffset + i*4,10)+QString("($t0)"));
                            }
                        }
                        if(loadedNumber != 0)
                            dynamicOutFileList.append(QString("\tli\t$t1, 0x")+QString::number(loadedNumber,16));
                        previousLoadedNumber = loadedNumber;
                        consecutiveValues = 0;
                    }else{
                        consecutiveValues++;
                    }
                    loadedNumber = 0;
                }
                byteNumber++;

            }else{
                storeByte(spriteRamBaseAddress + i, *byte);
            }if(count %1024 == 0)
                emit loadingNumberChanged(count / 1024);
            count++;
        }

        if(dynamic){
            if(consecutiveValues > 0){
                if(consecutiveValues > 6){
                    Uint32 startAddress = (byteNumber)-(consecutiveValues * 4)+liBaseAddress;
                    dynamicOutFileList.append("");
                    dynamicOutFileList.append(QString("\taddiu\t$t6, $zero, 0x")+QString::number(startAddress,16));
                    dynamicOutFileList.append(QString("loop") + QString::number(loopsCount) + QString(":") + QString("\tslti\t$t5, $t6, 0x")+QString::number(startAddress + (consecutiveValues * 4),16));
                    dynamicOutFileList.append(QString("\tbeq\t$t5, $zero, exit") + QString::number(loopsCount));
                    if(previousLoadedNumber == 0)
                        dynamicOutFileList.append(QString("\tsw\t$zero, 0($t6)"));
                    else
                        dynamicOutFileList.append(QString("\tsw\t$t1, 0($t6)"));
                    dynamicOutFileList.append(QString("\taddiu\t$t6, $t6, 4"));
                    dynamicOutFileList.append(QString("\tj\tloop") + QString::number(loopsCount));
                    dynamicOutFileList.append(QString("exit") + QString::number(loopsCount) + ":");
                    dynamicOutFileList.append("");
                    loopsCount++;
                }else{
                    Uint32 startOffset = (byteNumber-3)-(consecutiveValues * 4);
                    for(int i=0; i<consecutiveValues+1; i++){
                        dynamicOutFileList.append(QString("\tsw\t$t1, ")+QString::number(startOffset + i*4,10)+QString("($t0)"));
                    }
                }
            }
        }
    }
    qDebug() << count;

    if(segmentsToLoad[6]){
        int byteNumber = 0;
        loadedNumber = previousLoadedNumber = consecutiveValues = 0;
        uint liBaseAddress = this->paletteBaseAddress;
        if(dynamic){
            dynamicOutFileList.append("# Dumping palette");
            dynamicOutFileList.append(QString("\tli\t$t0, 0x")+QString::number(liBaseAddress,16)+QString("\t# Palette base address"));
            dynamicOutFileList.append("");
        }
        for(int i=0; i<palettePhysicalSize; i++){
            *byte = 1;
            in.getChar(byte);
            if(dynamic){
                if(MainWindow::isLittleEndian()){
                    loadedNumber |= (uint((unsigned char)(*byte))<<((byteNumber%4)*8));
                }else{
                    loadedNumber |= (uint((unsigned char)(*byte))<<((3-(byteNumber%4))*8));
                }if(byteNumber%4 == 3){
                    if(loadedNumber != previousLoadedNumber){
                        if(consecutiveValues > 6){
                            Uint32 startAddress = (byteNumber-3)-(consecutiveValues * 4)+liBaseAddress;
                            dynamicOutFileList.append("");
                            dynamicOutFileList.append(QString("\taddiu\t$t6, $zero, 0x")+QString::number(startAddress,16));
                            dynamicOutFileList.append(QString("loop") + QString::number(loopsCount) + QString(":") + QString("\tslti\t$t5, $t6, 0x")+QString::number(startAddress + (consecutiveValues * 4),16));
                            dynamicOutFileList.append(QString("\tbeq\t$t5, $zero, exit") + QString::number(loopsCount));
                            if(previousLoadedNumber == 0)
                                dynamicOutFileList.append(QString("\tsw\t$zero, 0($t6)"));
                            else
                                dynamicOutFileList.append(QString("\tsw\t$t1, 0($t6)"));
                            dynamicOutFileList.append(QString("\taddiu\t$t6, $t6, 4"));
                            dynamicOutFileList.append(QString("\tj\tloop") + QString::number(loopsCount));
                            dynamicOutFileList.append(QString("exit") + QString::number(loopsCount) + ":");
                            dynamicOutFileList.append("");
                            loopsCount++;
                        }else{
                            Uint32 startOffset = (byteNumber-3)-(consecutiveValues * 4);
                            for(int i=0; i<consecutiveValues+1; i++){
                                dynamicOutFileList.append(QString("\tsw\t$t1, ")+QString::number(startOffset + i*4,10)+QString("($t0)"));
                            }
                        }
                        if(loadedNumber != 0)
                            dynamicOutFileList.append(QString("\tli\t$t1, 0x")+QString::number(loadedNumber,16));
                        previousLoadedNumber = loadedNumber;
                        consecutiveValues = 0;
                    }else{
                        consecutiveValues++;
                    }
                    loadedNumber = 0;
                }
                byteNumber++;

            }else{
                storeByte(paletteBaseAddress + i, *byte);
            }if(count %1024 == 0)
                emit loadingNumberChanged(count / 1024);
            count++;
        }

        if(dynamic){
            if(consecutiveValues > 0){
                if(consecutiveValues > 6){
                    Uint32 startAddress = (byteNumber)-(consecutiveValues * 4)+liBaseAddress;
                    dynamicOutFileList.append("");
                    dynamicOutFileList.append(QString("\taddiu\t$t6, $zero, 0x")+QString::number(startAddress,16));
                    dynamicOutFileList.append(QString("loop") + QString::number(loopsCount) + QString(":") + QString("\tslti\t$t5, $t6, 0x")+QString::number(startAddress + (consecutiveValues * 4),16));
                    dynamicOutFileList.append(QString("\tbeq\t$t5, $zero, exit") + QString::number(loopsCount));
                    if(previousLoadedNumber == 0)
                        dynamicOutFileList.append(QString("\tsw\t$zero, 0($t6)"));
                    else
                        dynamicOutFileList.append(QString("\tsw\t$t1, 0($t6)"));
                    dynamicOutFileList.append(QString("\taddiu\t$t6, $t6, 4"));
                    dynamicOutFileList.append(QString("\tj\tloop") + QString::number(loopsCount));
                    dynamicOutFileList.append(QString("exit") + QString::number(loopsCount) + ":");
                    dynamicOutFileList.append("");
                    loopsCount++;
                }else{
                    Uint32 startOffset = (byteNumber-3)-(consecutiveValues * 4);
                    for(int i=0; i<consecutiveValues+1; i++){
                        dynamicOutFileList.append(QString("\tsw\t$t1, ")+QString::number(startOffset + i*4,10)+QString("($t0)"));
                    }
                }
            }
        }
    }

    if(dynamic){
        qDebug() << "dynamic dumping";
        ofstream myFile("D:/testFile.txt");
        for(QString line: dynamicOutFileList ){
            myFile << line.toStdString() << std::endl;
        }
        myFile.close();
    }

    qDebug() << "loaded: "<<count<<" bytes\n";
    in.close();
    emit loadingNumberChanged((claculateLoadSize(segmentsToLoad) + 1024 - 1)/1024);


}
*/
