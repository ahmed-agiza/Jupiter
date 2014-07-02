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
    instructionsmodel.cpp

HEADERS  += mainwindow.h \
    syntaxhl.h \
    globalRegex.h \
    codeeditor.h \
    instruction.h \
    assembler.h \
    memory.h \
    InstructionFuncs.h \
    registersmodel.h \
    memorymodel.h \
    instructionsmodel.h

FORMS    += \
    mainwindow.ui

RESOURCES += \
    qdarkstyle/style.qrc
