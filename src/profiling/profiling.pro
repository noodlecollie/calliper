#-------------------------------------------------
#
# Project created by QtCreator 2017-08-11T18:23:56
#
#-------------------------------------------------

TARGET = profiling
TEMPLATE = lib

DEFINES += PROFILING_LIBRARY

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
    profiling_global.cpp \
    profiling/profilermodel/profilermodel.cpp \
    profiling/profilermodel/profileritemmodeladatper.cpp

HEADERS += \
        profiling_global.h \ 
    profiling/scoped/scopedprofiler.h \
    profiling/profilermodel/profilermodel.h \
    profiling/profilermodel/profileritemmodeladatper.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
