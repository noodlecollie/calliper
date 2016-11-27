#-------------------------------------------------
#
# Project created by QtCreator 2016-10-11T08:16:00
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = vmfviewer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    simplerenderpassclassifier.cpp

HEADERS  += mainwindow.h \
    simplerenderpassclassifier.h

FORMS    +=

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../calliperutil/release/ -lcalliperutil
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../calliperutil/debug/ -lcalliperutil
else:unix: LIBS += -L$$OUT_PWD/../calliperutil/ -lcalliperutil

INCLUDEPATH += $$PWD/../calliperutil
DEPENDPATH += $$PWD/../calliperutil

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../renderer/release/ -lrenderer
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../renderer/debug/ -lrenderer
else:unix: LIBS += -L$$OUT_PWD/../renderer/ -lrenderer

INCLUDEPATH += $$PWD/../renderer
DEPENDPATH += $$PWD/../renderer

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../model/release/ -lmodel
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../model/debug/ -lmodel
else:unix: LIBS += -L$$OUT_PWD/../model/ -lmodel

INCLUDEPATH += $$PWD/../model
DEPENDPATH += $$PWD/../model

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../file-formats/release/ -lfile-formats
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../file-formats/debug/ -lfile-formats
else:unix: LIBS += -L$$OUT_PWD/../file-formats/ -lfile-formats

INCLUDEPATH += $$PWD/../file-formats
DEPENDPATH += $$PWD/../file-formats

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../map-importers/release/ -lmap-importers
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../map-importers/debug/ -lmap-importers
else:unix: LIBS += -L$$OUT_PWD/../map-importers/ -lmap-importers

INCLUDEPATH += $$PWD/../map-importers
DEPENDPATH += $$PWD/../map-importers
