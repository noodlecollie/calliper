#-------------------------------------------------
#
# Project created by QtCreator 2016-10-24T17:44:53
#
#-------------------------------------------------

TARGET = serialisation
TEMPLATE = lib

DEFINES += SERIALISATION_LIBRARY

SOURCES += \
    keyvaluesparser.cpp \
    keyvaluestoken.cpp

HEADERS +=\
        serialisation_global.h \
    keyvaluesparser.h \
    keyvaluestoken.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../model/release/ -lmodel
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../model/debug/ -lmodel
else:unix: LIBS += -L$$OUT_PWD/../model/ -lmodel

INCLUDEPATH += $$PWD/../model
DEPENDPATH += $$PWD/../model
