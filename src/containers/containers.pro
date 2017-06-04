#-------------------------------------------------
#
# Project created by QtCreator 2017-06-04T11:01:48
#
#-------------------------------------------------

QT       -= gui

TARGET = containers
TEMPLATE = lib

DEFINES += CONTAINERS_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    containers/containers_global.cpp

HEADERS +=\
        containers/containers_global.h \
    containers/item/itembasedobjectstore.h \
    containers/item/objectstoreitem.h \
    containers/itempointer/itempointerbasedobjectstore.h \
    containers/itempointer/objectstoreitempointer.h \
    containers/objectstore/objectstore.h \
    containers/path-managing/pathmanagingobjectstore.h \
    containers/static/staticobjectstore.h \
    containers/static/staticobjectstorekey.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
