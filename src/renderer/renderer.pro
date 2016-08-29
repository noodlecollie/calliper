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
    demoglwindow.cpp \
    openglerrors.cpp \
    rendermodelbatch.cpp \
    rendermodelbatchparams.cpp \
    rendermodelbatchitem.cpp

HEADERS +=\
        renderer_global.h \
    demoglwindow.h \
    openglerrors.h \
    rendermodelbatch.h \
    shaderdefs.h \
    rendermodelbatchparams.h \
    openglhelpers.h \
    rendermodelbatchitem.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
