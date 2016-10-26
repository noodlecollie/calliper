#-------------------------------------------------
#
# Project created by QtCreator 2016-10-24T17:44:53
#
#-------------------------------------------------

TARGET = serialisation
TEMPLATE = lib

DEFINES += SERIALISATION_LIBRARY

SOURCES += \
    keyvalues/keyvaluesparser.cpp \
    keyvalues/keyvaluestoken.cpp \
    vmf/vmf.cpp \
    json/jsonarraywrapper.cpp \
    json/jsonutil.cpp

HEADERS +=\
        serialisation_global.h \
    keyvalues/keyvaluesparser.h \
    keyvalues/keyvaluestoken.h \
    vmf/vmf.h \
    json/jsonarraywrapper.h \
    json/jsonutil.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../model/release/ -lmodel
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../model/debug/ -lmodel
else:unix: LIBS += -L$$OUT_PWD/../model/ -lmodel

INCLUDEPATH += $$PWD/../model
DEPENDPATH += $$PWD/../model

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../renderer/release/ -lrenderer
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../renderer/debug/ -lrenderer
else:unix: LIBS += -L$$OUT_PWD/../renderer/ -lrenderer

INCLUDEPATH += $$PWD/../renderer
DEPENDPATH += $$PWD/../renderer

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../calliperutil/release/ -lcalliperutil
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../calliperutil/debug/ -lcalliperutil
else:unix: LIBS += -L$$OUT_PWD/../calliperutil/ -lcalliperutil

INCLUDEPATH += $$PWD/../calliperutil
DEPENDPATH += $$PWD/../calliperutil
