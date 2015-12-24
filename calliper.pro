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
    viewport.cpp \
    resourcemanager.cpp \
    temporaryrender.cpp \
    shaderprogram.cpp \
    minimumshader.cpp \
    openglrenderer.cpp

HEADERS  += mainwindow.h \
    viewport.h \
    resourcemanager.h \
    temporaryrender.h \
    shaderprogram.h \
    minimumshader.h \
    openglrenderer.h

FORMS    += mainwindow.ui

RESOURCES += \
    resource/resource.qrc
