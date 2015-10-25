#-------------------------------------------------
#
# Project created by QtCreator 2015-10-24T08:34:07
#
#-------------------------------------------------

CONFIG -= qt
TARGET = readmetrics
TEMPLATE = lib

DEFINES += READMETRICS_LIBRARY

SOURCES += Readmetrics.cpp

HEADERS +=\
        Metrics_def.h \
    ReadMetrics.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
