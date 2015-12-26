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
    openglrenderer.cpp \
    geometrydata.cpp \
    minimumtexturedshader.cpp \
    geometryfactory.cpp \
    sceneobject.cpp \
    callipermath.cpp \
    scene.cpp \
    unlittextureshader.cpp \
    cameralens.cpp \
    camera.cpp \
    eulerangle.cpp \
    cameracontroller.cpp \
    pervertexcolorshader.cpp \
    basiclittextureshader.cpp

HEADERS  += mainwindow.h \
    viewport.h \
    resourcemanager.h \
    temporaryrender.h \
    shaderprogram.h \
    minimumshader.h \
    openglrenderer.h \
    geometrydata.h \
    minimumtexturedshader.h \
    geometryfactory.h \
    sceneobject.h \
    callipermath.h \
    scene.h \
    unlittextureshader.h \
    cameralens.h \
    camera.h \
    eulerangle.h \
    cameracontroller.h \
    pervertexcolorshader.h \
    basiclittextureshader.h

FORMS    += mainwindow.ui

RESOURCES += \
    resource/resource.qrc
