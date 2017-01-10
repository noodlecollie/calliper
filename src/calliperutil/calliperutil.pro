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
    general/hashfunctions.cpp \
    general/fixedindexpool.cpp \
    exceptions/calliperexception.cpp \
    json/jsonarraywrapper.cpp \
    calliperutil_global.cpp \
    string/stringutil.cpp

HEADERS +=\
        calliperutil_global.h \
    general/generalutil.h \
    math/math.h \
    general/hashfunctions.h \
    general/fixedindexpool.h \
    exceptions/calliperexception.h \
    json/jsonarraywrapper.h \
    string/stringutil.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
