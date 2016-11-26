#-------------------------------------------------
#
# Project created by QtCreator 2016-11-26T18:30:01
#
#-------------------------------------------------

QT       -= gui

TARGET = dep-qvtf
TEMPLATE = lib

DEFINES += DEPQVTF_LIBRARY

SOURCES +=

HEADERS +=\
        dep-qvtf_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
