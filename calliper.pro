#-------------------------------------------------
#
# Project created by QtCreator 2015-09-27T21:39:33
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = calliper
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    cviewport.cpp \
    cbasiccamera.cpp

HEADERS  += mainwindow.h \
    cviewport.h \
    cbasiccamera.h

FORMS    += mainwindow.ui

RESOURCES += \
    resource.qrc
