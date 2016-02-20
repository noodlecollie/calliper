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
    shaderprogram.cpp \
    minimumshader.cpp \
    openglrenderer.cpp \
    geometrydata.cpp \
    minimumtexturedshader.cpp \
    geometryfactory.cpp \
    sceneobject.cpp \
    callipermath.cpp \
    unlittextureshader.cpp \
    cameralens.cpp \
    eulerangle.cpp \
    cameracontroller.cpp \
    pervertexcolorshader.cpp \
    basiclittextureshader.cpp \
    matrixstack.cpp \
    mapdocument.cpp \
    scenefactory.cpp \
    originmarker.cpp \
    application.cpp \
    viewportuseroptions.cpp \
    simplenumericfont.cpp \
    boundingbox.cpp \
    shaderstack.cpp \
    selectionmaskshader.cpp \
    basetool.cpp \
    sceneobjectmanipulator.cpp \
    debugtesttool.cpp \
    objfileparser.cpp \
    inputprocessor.cpp \
    scenecamera.cpp \
    hierarchicalobject.cpp \
    mapscene.cpp \
    basescene.cpp \
    uiscene.cpp \
    mapgrid.cpp \
    cameraparams.cpp \
    translationhandle.cpp \
    uimanipulator.cpp \
    translationtool.cpp \
    ray3d.cpp \
    jsonutil.cpp \
    callipermapfile.cpp \
    generalutil.cpp \
    sceneobjectfactory.cpp

HEADERS  += mainwindow.h \
    viewport.h \
    resourcemanager.h \
    shaderprogram.h \
    minimumshader.h \
    openglrenderer.h \
    geometrydata.h \
    minimumtexturedshader.h \
    geometryfactory.h \
    sceneobject.h \
    callipermath.h \
    unlittextureshader.h \
    cameralens.h \
    eulerangle.h \
    cameracontroller.h \
    pervertexcolorshader.h \
    basiclittextureshader.h \
    matrixstack.h \
    mapdocument.h \
    factories.h \
    scenefactory.h \
    originmarker.h \
    application.h \
    shaders.h \
    viewportuseroptions.h \
    simplenumericfont.h \
    boundingbox.h \
    shaderstack.h \
    selectionmaskshader.h \
    basetool.h \
    sceneobjectmanipulator.h \
    tools.h \
    debugtesttool.h \
    objfileparser.h \
    inputprocessor.h \
    scenecamera.h \
    hierarchicalobject.h \
    mapscene.h \
    basescene.h \
    uiscene.h \
    mapgrid.h \
    cameraparams.h \
    translationhandle.h \
    uimanipulator.h \
    translationtool.h \
    ray3d.h \
    iserialisable.h \
    jsonutil.h \
    callipermapfile.h \
    generalutil.h \
    sceneobjectfactory.h

FORMS    += mainwindow.ui \
    viewportuseroptions.ui

RESOURCES += \
    resource/resource.qrc
