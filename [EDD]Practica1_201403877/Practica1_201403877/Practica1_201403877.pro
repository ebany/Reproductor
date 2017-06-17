#-------------------------------------------------
#
# Project created by QtCreator 2017-06-07T19:46:01
#
#-------------------------------------------------

QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Practica1_201403877
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    biblioteca.cpp

HEADERS  += mainwindow.h \
    biblioteca.h

FORMS    += mainwindow.ui

QMAKE_CXXFLAGS += -std=gnu++11
