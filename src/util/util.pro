#-------------------------------------------------
#
# Project created by QtCreator 2016-09-03T23:35:32
#
#-------------------------------------------------

QT       += opengl

TARGET = util
TEMPLATE = lib

DEFINES += UTIL_LIBRARY

SOURCES += \
    general/generalutil.cpp \
    math/math.cpp

HEADERS +=\
        util_global.h \
    general/generalutil.h \
    math/math.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
