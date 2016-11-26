#-------------------------------------------------
#
# Project created by QtCreator 2016-11-26T18:30:01
#
#-------------------------------------------------

QT       += gui

TARGET = dep-qvtf
TEMPLATE = lib

DEFINES += DEPQVTF_LIBRARY

SOURCES += \
    qvtf/ValveTextureFile.cpp

HEADERS +=\
        dep-qvtf_global.h \
    qvtf/ValveTextureFile.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../dep-vtflib/release/ -ldep-vtflib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../dep-vtflib/debug/ -ldep-vtflib
else:unix: LIBS += -L$$OUT_PWD/../dep-vtflib/ -ldep-vtflib

INCLUDEPATH += $$PWD/../dep-vtflib
DEPENDPATH += $$PWD/../dep-vtflib

DISTFILES += \
    qvtf/ValveTextureFilePlugin.json
