#-------------------------------------------------
#
# Project created by QtCreator 2014-06-26T20:18:44
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Jupiter
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    syntaxhl.cpp \
    codeeditor.cpp \
    instruction.cpp \
    assembler.cpp \
    memory.cpp \
    InstructionFuncs.cpp \
    registersmodel.cpp \
    memorymodel.cpp \
    instructionsmodel.cpp \
    palettecolor.cpp \
    tile.cpp \
    gamesprite.cpp \
    tileengine.cpp \
    qrenderwindow.cpp \
    loadmemorythread.cpp \
    memoryloading.cpp

HEADERS  += mainwindow.h \
    syntaxhl.h \
    codeeditor.h \
    instruction.h \
    assembler.h \
    memory.h \
    InstructionFuncs.h \
    registersmodel.h \
    memorymodel.h \
    instructionsmodel.h \
    palettecolor.h \
    tile.h \
    gamesprite.h \
    tileengine.h \
    qrenderwindow.h \
    loadmemorythread.h \
    memoryloading.h

FORMS    += \
    mainwindow.ui \
    memoryloading.ui

RESOURCES += \
    qdarkstyle/style.qrc

LIBS += -LC:/SFML/lib

CONFIG(release, debug|release): LIBS += -lsfml-audio -lsfml-graphics -lsfml-main -lsfml-network -lsfml-window -lsfml-system
CONFIG(debug, debug|release): LIBS += -lsfml-audio-d -lsfml-graphics-d -lsfml-main-d -lsfml-network-d -lsfml-window-d -lsfml-system-d

INCLUDEPATH += C:/SFML/include
DEPENDPATH += C:/SFML/include

CONFIG += c++11
