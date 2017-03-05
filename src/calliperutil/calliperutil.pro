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
    calliperutil/exceptions/calliperexception.cpp \
    calliperutil/general/generalutil.cpp \
    calliperutil/general/hashfunctions.cpp \
    calliperutil/json/jsonarraywrapper.cpp \
    calliperutil/math/math.cpp \
    calliperutil/string/stringutil.cpp \
    calliperutil_global.cpp \
    calliperutil/qobject/qobjectutil.cpp

HEADERS +=\
        calliperutil_global.h \
    calliperutil/exceptions/calliperexception.h \
    calliperutil/general/generalutil.h \
    calliperutil/general/hashfunctions.h \
    calliperutil/json/jsonarraywrapper.h \
    calliperutil/math/math.h \
    calliperutil/string/stringutil.h \
    calliperutil/debug/debug.h \
    calliperutil/qobject/qobjectutil.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
