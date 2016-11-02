QT += core
QT -= gui

CONFIG += c++11

TARGET = vpkinfo
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    vpkinfo.cpp

HEADERS += \
    vpkinfo.h

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../file-formats/release/ -lfile-formats
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../file-formats/debug/ -lfile-formats
else:unix: LIBS += -L$$OUT_PWD/../file-formats/ -lfile-formats

INCLUDEPATH += $$PWD/../file-formats
DEPENDPATH += $$PWD/../file-formats
