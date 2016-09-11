#-------------------------------------------------
#
# Project created by QtCreator 2016-09-03T23:35:32
#
#-------------------------------------------------

QT       += opengl

TARGET = calliperutil
TEMPLATE = lib

DEFINES += UTIL_LIBRARY

SOURCES += \
    general/generalutil.cpp \
    math/math.cpp \
    json/jsonutil.cpp \
    general/hashfunctions.cpp

HEADERS +=\
        calliperutil_global.h \
    general/generalutil.h \
    math/math.h \
    json/jsonutil.h \
    general/hashfunctions.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
