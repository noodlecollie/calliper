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
    file-formats_global.cpp \
    file-formats/common/streamdatacontainer.cpp \
    file-formats/keyvalues/keyvaluesparser.cpp \
    file-formats/keyvalues/keyvaluestoken.cpp \
    file-formats/vpk/vpkarchivemd5collection.cpp \
    file-formats/vpk/vpkarchivemd5item.cpp \
    file-formats/vpk/vpkfile.cpp \
    file-formats/vpk/vpkfilecollection.cpp \
    file-formats/vpk/vpkheader.cpp \
    file-formats/vpk/vpkindex.cpp \
    file-formats/vpk/vpkindextreeitem.cpp \
    file-formats/vpk/vpkindextreeiterator.cpp \
    file-formats/vpk/vpkindextreerecord.cpp \
    file-formats/vpk/vpkothermd5item.cpp

HEADERS +=\
        file-formats_global.h \
    file-formats/collection/simpleitemcollection.h \
    file-formats/common/streamdatacontainer.h \
    file-formats/keyvalues/keyvaluesparser.h \
    file-formats/keyvalues/keyvaluestoken.h \
    file-formats/vpk/vpkarchivemd5collection.h \
    file-formats/vpk/vpkarchivemd5item.h \
    file-formats/vpk/vpkfile.h \
    file-formats/vpk/vpkfilecollection.h \
    file-formats/vpk/vpkheader.h \
    file-formats/vpk/vpkindex.h \
    file-formats/vpk/vpkindextreeitem.h \
    file-formats/vpk/vpkindextreeiterator.h \
    file-formats/vpk/vpkindextreerecord.h \
    file-formats/vpk/vpkothermd5item.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../calliperutil/release/ -lcalliperutil
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../calliperutil/debug/ -lcalliperutil
else:unix: LIBS += -L$$OUT_PWD/../calliperutil/ -lcalliperutil

INCLUDEPATH += $$PWD/../calliperutil
DEPENDPATH += $$PWD/../calliperutil
