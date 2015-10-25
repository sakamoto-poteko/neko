#-------------------------------------------------
#
# Project created by QtCreator 2015-10-23T04:50:05
#
#-------------------------------------------------

QT       += core

QT       -= gui

CONFIG   += console c++11
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    Metrics.cpp

HEADERS += \
    Metrics.h \
    Metrics_def.h

DEFINES += "LOGGER_LEVEL=LL_WARN"

QMAKE_CXXFLAGS += -g
QMAKE_LFLAGS += -g
