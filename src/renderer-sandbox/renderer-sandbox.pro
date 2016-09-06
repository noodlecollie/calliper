#-------------------------------------------------
#
# Project created by QtCreator 2016-08-17T21:23:43
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = renderer-sandbox
TEMPLATE = app


SOURCES += main.cpp \
    demoglwindow.cpp \
    tempshader.cpp

HEADERS  += \
    demoglwindow.h \
    tempshader.h

FORMS    +=

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../renderer/release/ -lrenderer
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../renderer/debug/ -lrenderer
else:unix: LIBS += -L$$OUT_PWD/../renderer/ -lrenderer

INCLUDEPATH += $$PWD/../renderer
DEPENDPATH += $$PWD/../renderer

RESOURCES += \
    resource.qrc

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../util/release/ -lutil
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../util/debug/ -lutil
else:unix: LIBS += -L$$OUT_PWD/../util/ -lutil

INCLUDEPATH += $$PWD/../util
DEPENDPATH += $$PWD/../util
