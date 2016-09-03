#-------------------------------------------------
#
# Project created by QtCreator 2016-09-03T22:28:04
#
#-------------------------------------------------

QT       += opengl

TARGET = model
TEMPLATE = lib

DEFINES += MODEL_LIBRARY

SOURCES += \
    scene/basescene.cpp \
    scene/mapscene.cpp \
    scene/sceneobject.cpp \
    scene/uiscene.cpp \
    scene/hierarchicalobject.cpp \
    scene/spatialsnapshot.cpp \
    scene/textureplane.cpp \
    sceneobjects/axisdraghandle.cpp \
    sceneobjects/basedraghandle.cpp \
    sceneobjects/blockcreationhandle.cpp \
    sceneobjects/brush.cpp \
    sceneobjects/brushface.cpp \
    sceneobjects/crosshair3d.cpp \
    sceneobjects/mapgrid.cpp \
    sceneobjects/originmarker.cpp \
    sceneobjects/scalehandle.cpp \
    sceneobjects/scenecamera.cpp \
    sceneobjects/translationhandle.cpp \
    sceneobjects/uiblockcreationhandle.cpp \
    sceneobjects/uimanipulator.cpp

HEADERS +=\
        model_global.h \
    scene/basescene.h \
    scene/mapscene.h \
    scene/sceneobject.h \
    scene/uiscene.h \
    scene/hierarchicalobject.h \
    scene/spatialsnapshot.h \
    scene/textureplane.h \
    sceneobjects/axisdraghandle.h \
    sceneobjects/basedraghandle.h \
    sceneobjects/blockcreationhandle.h \
    sceneobjects/brush.h \
    sceneobjects/brushface.h \
    sceneobjects/crosshair3d.h \
    sceneobjects/mapgrid.h \
    sceneobjects/originmarker.h \
    sceneobjects/scalehandle.h \
    sceneobjects/scenecamera.h \
    sceneobjects/translationhandle.h \
    sceneobjects/uiblockcreationhandle.h \
    sceneobjects/uimanipulator.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
