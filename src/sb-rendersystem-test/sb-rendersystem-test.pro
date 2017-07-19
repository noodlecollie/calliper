#-------------------------------------------------
#
# Project created by QtCreator 2017-06-05T08:15:50
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = sb-rendersystem-test
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../calliperutil/release/ -lcalliperutil
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../calliperutil/debug/ -lcalliperutil
else:unix: LIBS += -L$$OUT_PWD/../calliperutil/ -lcalliperutil

INCLUDEPATH += $$PWD/../calliperutil
DEPENDPATH += $$PWD/../calliperutil

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../rendersystem/release/ -lrendersystem
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../rendersystem/debug/ -lrendersystem
else:unix: LIBS += -L$$OUT_PWD/../rendersystem/ -lrendersystem

INCLUDEPATH += $$PWD/../rendersystem
DEPENDPATH += $$PWD/../rendersystem

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../containers/release/ -lcontainers
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../containers/debug/ -lcontainers
else:unix: LIBS += -L$$OUT_PWD/../containers/ -lcontainers

INCLUDEPATH += $$PWD/../containers
DEPENDPATH += $$PWD/../containers
