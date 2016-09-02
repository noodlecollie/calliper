#-------------------------------------------------
#
# Project created by QtCreator 2016-08-17T21:21:29
#
#-------------------------------------------------

QT       += opengl

TARGET = renderer
TEMPLATE = lib

DEFINES += RENDERER_LIBRARY

SOURCES += \
    openglerrors.cpp \
    rendermodelbatch.cpp \
    rendermodelbatchparams.cpp \
    rendermodelbatchitem.cpp \
    opengluniformbuffer.cpp \
    shaderdefs.cpp \
    openglshaderprogram.cpp \
    rendermodelbatchkey.cpp \
    rendermodel.cpp \
    shaders/debugscreenspaceshader.cpp \
    shaderstore.cpp

HEADERS +=\
        renderer_global.h \
    openglerrors.h \
    rendermodelbatch.h \
    shaderdefs.h \
    rendermodelbatchparams.h \
    openglhelpers.h \
    rendermodelbatchitem.h \
    ishaderspec.h \
    opengluniformbuffer.h \
    openglshaderprogram.h \
    rendermodelbatchkey.h \
    rendermodel.h \
    shaders/debugscreenspaceshader.h \
    shaderstore.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

RESOURCES += \
    resource/resource.qrc
