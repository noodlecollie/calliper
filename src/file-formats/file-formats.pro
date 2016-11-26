#-------------------------------------------------
#
# Project created by QtCreator 2016-11-02T08:13:09
#
#-------------------------------------------------

QT       -= gui

TARGET = file-formats
TEMPLATE = lib

DEFINES += FILEFORMATS_LIBRARY

SOURCES += \
    keyvalues/keyvaluestoken.cpp \
    keyvalues/keyvaluesparser.cpp \
    vpk/vpkheader.cpp \
    vpk/vpkindextreeitem.cpp \
    vpk/vpkindex.cpp \
    vpk/vpkindextreerecord.cpp \
    vpk/vpkfile.cpp \
    vpk/vpkarchivemd5item.cpp \
    common/streamdatacontainer.cpp \
    vpk/vpkothermd5item.cpp \
    vpk/vpkarchivemd5collection.cpp \
    vpk/vpkfilecollection.cpp

HEADERS +=\
        file-formats_global.h \
    keyvalues/keyvaluestoken.h \
    keyvalues/keyvaluesparser.h \
    vpk/vpkheader.h \
    vpk/vpkindextreeitem.h \
    vpk/vpkindex.h \
    vpk/vpkindextreerecord.h \
    vpk/vpkfile.h \
    vpk/vpkarchivemd5item.h \
    common/streamdatacontainer.h \
    vpk/vpkothermd5item.h \
    vpk/vpkarchivemd5collection.h \
    collection/simpleitemcollection.h \
    vpk/vpkfilecollection.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../calliperutil/release/ -lcalliperutil
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../calliperutil/debug/ -lcalliperutil
else:unix: LIBS += -L$$OUT_PWD/../calliperutil/ -lcalliperutil

INCLUDEPATH += $$PWD/../calliperutil
DEPENDPATH += $$PWD/../calliperutil
