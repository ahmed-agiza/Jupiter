#include "initializememorythread.h"

InitializeMemoryThread::InitializeMemoryThread(Memory *memory, QObject *parent) :
    QThread(parent)
{
    this->memory = memory;
}

void InitializeMemoryThread::run()
{
    memory->initializeGPUMemory();
    emit loadComplete();
}
