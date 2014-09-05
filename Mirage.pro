#-------------------------------------------------
#
# Project created by QtCreator 2014-06-26T20:18:44
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Mirage
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
    memoryloading.cpp \
    tilesetviewer.cpp \
    tilerenderwindow.cpp \
    paletteviewer.cpp \
    paletterenderwindow.cpp \
    tileloader.cpp \
    spriteviewer.cpp \
    spriterenderwindow.cpp \
    inputmanager.cpp \
    codeeditorwindow.cpp \
    fileloader.cpp \
    explorertreeitem.cpp \
    startupdialog.cpp \
    projectcreator.cpp \
    keyboardmanager.cpp \
    filenamedialog.cpp \
    deletefiledialog.cpp \
    completerlist.cpp \
    ioconsole.cpp \
    memorydump.cpp \
    savememorythread.cpp \
    gpumemorydump.cpp


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
    memoryloading.h \
    tilesetviewer.h \
    tilerenderwindow.h \
    paletteviewer.h \
    paletterenderwindow.h \
    trie.h \
    tileloader.h \
    spriteviewer.h \
    spriterenderwindow.h \
    inputmanager.h \
    codeeditorwindow.h \
    fileloader.h \
    explorertreeitem.h \
    startupdialog.h \
    projectcreator.h \
    keyboardmanager.h \
    filenamedialog.h \
    deletefiledialog.h \
    completerlist.h \
    ioconsole.h \
    memorydump.h \
    savememorythread.h \
    gpumemorydump.h


FORMS    += \
    mainwindow.ui \
    memoryloading.ui \
    tilesetviewer.ui \
    paletteviewer.ui \
    tileloader.ui \
    spriteviewer.ui \
    inputmanager.ui \
    fileloader.ui \
    startupdialog.ui \
    projectcreator.ui \
    filenamedialog.ui \
    deletefiledialog.ui \
    memorydump.ui \
    gpumemorydump.ui

RESOURCES += \
    qdarkstyle/style.qrc \
    res.qrc

LIBS += -LC:/SFML/lib

CONFIG(release, debug|release): LIBS += -lsfml-audio -lsfml-graphics -lsfml-main -lsfml-network -lsfml-window -lsfml-system
CONFIG(debug, debug|release): LIBS += -lsfml-audio-d -lsfml-graphics-d -lsfml-main-d -lsfml-network-d -lsfml-window-d -lsfml-system-d

INCLUDEPATH += C:/SFML/include
DEPENDPATH += C:/SFML/include

CONFIG += c++11

QMAKE_CFLAGS += -Wno-unused
QMAKE_CFLAGS += -Wno-unused-parameter

#win32:RC_ICONS += :/icons/icons/appicon.ico
win32:RC_ICONS += appicon.ico

#:/icons/icons/appicon.ico
