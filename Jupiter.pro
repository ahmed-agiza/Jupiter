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
    syntaxhl.cpp

HEADERS  += mainwindow.h \
    syntaxhl.h \
    globalRegex.h

FORMS    += \
    mainwindow.ui
