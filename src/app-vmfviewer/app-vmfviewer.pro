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

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../model-loaders/release/ -lmodel-loaders
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../model-loaders/debug/ -lmodel-loaders
else:unix: LIBS += -L$$OUT_PWD/../model-loaders/ -lmodel-loaders

INCLUDEPATH += $$PWD/../model-loaders
DEPENDPATH += $$PWD/../model-loaders

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../dep-vtflib/release/ -ldep-vtflib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../dep-vtflib/debug/ -ldep-vtflib
else:unix: LIBS += -L$$OUT_PWD/../dep-vtflib/ -ldep-vtflib

INCLUDEPATH += $$PWD/../dep-vtflib
DEPENDPATH += $$PWD/../dep-vtflib

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../user-interface/release/ -luser-interface
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../user-interface/debug/ -luser-interface
else:unix: LIBS += -L$$OUT_PWD/../user-interface/ -luser-interface

INCLUDEPATH += $$PWD/../user-interface
DEPENDPATH += $$PWD/../user-interface

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../rendersystem/release/ -lrendersystem
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../rendersystem/debug/ -lrendersystem
else:unix: LIBS += -L$$OUT_PWD/../rendersystem/ -lrendersystem

INCLUDEPATH += $$PWD/../rendersystem
DEPENDPATH += $$PWD/../rendersystem
