#include "loadmemorythread.h"

LoadMemoryThread::LoadMemoryThread(QObject *parent) :
    QThread(parent)
{

}

void LoadMemoryThread::run()
{
    QMutex mutex;

    QVector<bool> segmentsToLoad(8);
    segmentsToLoad.fill(0);
    segmentsToLoad[2] = 1;
    segmentsToLoad[3] = 1;
    segmentsToLoad[4] = 1;
    segmentsToLoad[5] = 1;
    segmentsToLoad[6] = 1;
    //memory->loadMemory("C:/memory.bin",segmentsToLoad);

    std::ifstream in;
    in.open("C:/memory.bin", std::ifstream::binary);
    char byte;

    if(segmentsToLoad[0]){
        for(int i=0; i<memory->textSegmentPhysicalSize; i++){
            in.get(byte);
            memory->storeByte(memory->textSegmentBaseAddress + i, byte);
            sleep(1);
        }
    }
    if(segmentsToLoad[1]){
        for(int i=0; i<memory->dataSegmentPhysicalSize; i++){
            in.get(byte);
            memory->storeByte(memory->dataSegmentBaseAddress + i, byte);
            sleep(1);
        }
    }

    if(segmentsToLoad[2]){
        for(int i=0; i<memory->backgroundTileSetPhysicalSize; i++){
            in.get(byte);
            memory->storeByte(memory->backgroundTileSetBaseAddress + i, byte);
            sleep(1);
        }
    }
    if(segmentsToLoad[3]){
        for(int i=0; i<memory->spritesTileSetPhysicalSize; i++){
            in.get(byte);
            memory->storeByte(memory->spritesTileSetBaseAddress + i, byte);
            sleep(1);
        }
    }
    if(segmentsToLoad[4]){
        for(int i=0; i<memory->tileMapPhysicalSize; i++){
            in.get(byte);
            memory->storeByte(memory->tileMapBaseAddress + i, byte);
            sleep(1);
        }
    }
    if(segmentsToLoad[5]){
        for(int i=0; i<memory->spriteRamPhysicalSize; i++){
            in.get(byte);
            memory->storeByte(memory->spriteRamBaseAddress + i, byte);
            sleep(1);
        }
    }
    if(segmentsToLoad[6]){
        for(int i=0; i<memory->palettePhysicalSize; i++){
            in.get(byte);
            memory->storeByte(memory->paletteBaseAddress + i, byte);
            sleep(1);
        }
    }
    if(segmentsToLoad[7]){
        for(int i=0; i<memory->heapSegmentPhysicalSize; i++){
            in.get(byte);
            memory->storeByte(memory->heapSegmentBaseAddress + i, byte);
            sleep(1);
        }
    }
    in.close();
}
