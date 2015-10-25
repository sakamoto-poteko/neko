#-------------------------------------------------
#
# Project created by QtCreator 2015-10-24T22:48:07
#
#-------------------------------------------------

QT       -= core gui

TARGET = Jieba
TEMPLATE = lib
CONFIG += staticlib

SOURCES += Jieba.cpp

HEADERS += Jieba.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}
