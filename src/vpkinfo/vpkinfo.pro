QT += core
QT -= gui

CONFIG += c++11

TARGET = vpkinfo
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../serialisation/release/ -lserialisation
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../serialisation/debug/ -lserialisation
else:unix: LIBS += -L$$OUT_PWD/../serialisation/ -lserialisation

INCLUDEPATH += $$PWD/../serialisation
DEPENDPATH += $$PWD/../serialisation
