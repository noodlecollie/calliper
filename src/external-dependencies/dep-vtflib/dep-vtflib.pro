#-------------------------------------------------
#
# Project created by QtCreator 2016-11-26T18:29:21
#
#-------------------------------------------------

QT       -= gui

TARGET = dep-vtflib
TEMPLATE = lib

DEFINES += DEPVTFLIB_LIBRARY

SOURCES +=

HEADERS +=\
        dep-vtflib_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
