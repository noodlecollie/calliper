#-------------------------------------------------
#
# Project created by QtCreator 2016-08-17T21:21:29
#
#-------------------------------------------------

QT       += opengl

TARGET = renderer
TEMPLATE = lib

DEFINES += RENDERER_LIBRARY

INCLUDEPATH += \
    opengl \
    shaders \
    rendermodel \
    shaderstore

SOURCES += \
    opengl/openglerrors.cpp \
    rendermodel/rendermodelbatch.cpp \
    rendermodel/rendermodelbatchparams.cpp \
    rendermodel/rendermodelbatchitem.cpp \
    opengl/opengluniformbuffer.cpp \
    shaders/shaderdefs.cpp \
    opengl/openglshaderprogram.cpp \
    rendermodel/rendermodelbatchkey.cpp \
    rendermodel/rendermodel.cpp \
    shaders/debugscreenspaceshader.cpp \
    shaderstore/shaderstore.cpp

HEADERS +=\
        renderer_global.h \
    opengl/openglerrors.h \
    rendermodel/rendermodelbatch.h \
    shaders/shaderdefs.h \
    rendermodel/rendermodelbatchparams.h \
    opengl/openglhelpers.h \
    rendermodel/rendermodelbatchitem.h \
    shaders/ishaderspec.h \
    opengl/opengluniformbuffer.h \
    opengl/openglshaderprogram.h \
    rendermodel/rendermodelbatchkey.h \
    rendermodel/rendermodel.h \
    shaders/debugscreenspaceshader.h \
    shaderstore/shaderstore.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

RESOURCES += \
    resource/resource.qrc
