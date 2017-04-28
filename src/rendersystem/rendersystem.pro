#-------------------------------------------------
#
# Project created by QtCreator 2017-04-26T12:44:58
#
#-------------------------------------------------

QT       += opengl

QT       -= gui

TARGET = rendersystem
TEMPLATE = lib

DEFINES += RENDERSYSTEM_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += rendersystem/global/rendersystem.cpp \
    rendersystem_global.cpp \
    rendersystem/interface-classes/currentcontextguard/contextreferencecounter.cpp \
    rendersystem/private/test/testinterface.cpp \
    rendersystem/endpoints/testinterfaceendpoint.cpp

HEADERS += rendersystem/global/rendersystem.h\
        rendersystem_global.h \
    rendersystem/interface-classes/store/objectstore.h \
    rendersystem/interface-classes/currentcontextguard/currentcontextguard.h \
    rendersystem/interface-classes/currentcontextguard/contextreferencecounter.h \
    rendersystem/interfaces/itestinterface.h \
    rendersystem/private/test/testinterface.h \
    rendersystem/endpoints/testinterfaceendpoint.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
