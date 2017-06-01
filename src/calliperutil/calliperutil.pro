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
    calliperutil/hash/hashfunctions.cpp \
    calliperutil/json/jsonarraywrapper.cpp \
    calliperutil/math/math.cpp \
    calliperutil/string/stringutil.cpp \
    calliperutil_global.cpp \
    calliperutil/qobject/qobjectutil.cpp \
    calliperutil/opengl/openglerrors.cpp \
    calliperutil/opengl/openglhelpers.cpp \
    calliperutil/paths/paths.cpp

HEADERS +=\
        calliperutil_global.h \
    calliperutil/exceptions/calliperexception.h \
    calliperutil/hash/hashfunctions.h \
    calliperutil/json/jsonarraywrapper.h \
    calliperutil/math/math.h \
    calliperutil/string/stringutil.h \
    calliperutil/debug/debug.h \
    calliperutil/qobject/qobjectutil.h \
    calliperutil/opengl/openglerrors.h \
    calliperutil/opengl/openglhelpers.h \
    calliperutil/paths/paths.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
