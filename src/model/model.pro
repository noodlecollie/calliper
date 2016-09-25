#-------------------------------------------------
#
# Project created by QtCreator 2016-09-03T22:28:04
#
#-------------------------------------------------

QT       += opengl

TARGET = model
TEMPLATE = lib

DEFINES += MODEL_LIBRARY

SOURCES += \
    tempremoveme.cpp

HEADERS += \
    tempremoveme.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../calliperutil/release/ -lcalliperutil
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../calliperutil/debug/ -lcalliperutil
else:unix: LIBS += -L$$OUT_PWD/../calliperutil/ -lcalliperutil

INCLUDEPATH += $$PWD/../calliperutil
DEPENDPATH += $$PWD/../calliperutil
