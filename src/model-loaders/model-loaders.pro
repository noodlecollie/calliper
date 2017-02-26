#-------------------------------------------------
#
# Project created by QtCreator 2016-11-02T08:27:28
#
#-------------------------------------------------

QT       += gui

TARGET = model-loaders
TEMPLATE = lib

DEFINES += MODELLOADERS_LIBRARY

SOURCES += \
    model-loaders/json/jsonloaderutils.cpp \
    model-loaders/projects/calliperprojectloader.cpp \
    model-loaders/vtf/vtfloader.cpp \
    model-loaders/filedataloaders/base/basefileloader.cpp \
    model-loaders/filedataloaders/vmf/vmfdataloader.cpp \
    model-loaders/filedataloaders/fileextensiondatamodelmap.cpp \
    model-loaders/filedataloaders/filedataloaderfactory.cpp
    model-loaders_global.cpp

HEADERS +=\
    model-loaders_global.h \
    model-loaders/json/jsonloaderutils.h \
    model-loaders/projects/calliperprojectloader.h \
    model-loaders/vtf/vtfloader.h \
    model-loaders/filedataloaders/base/basefileloader.h \
    model-loaders/filedataloaders/vmf/vmfdataloader.h \
    model-loaders/filedataloaders/fileextensiondatamodelmap.h \
    model-loaders/filedataloaders/filedataloaderfactory.h

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

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../file-formats/release/ -lfile-formats
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../file-formats/debug/ -lfile-formats
else:unix: LIBS += -L$$OUT_PWD/../file-formats/ -lfile-formats

INCLUDEPATH += $$PWD/../file-formats
DEPENDPATH += $$PWD/../file-formats

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../dep-vtflib/release/ -ldep-vtflib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../dep-vtflib/debug/ -ldep-vtflib
else:unix: LIBS += -L$$OUT_PWD/../dep-vtflib/ -ldep-vtflib

INCLUDEPATH += $$PWD/../dep-vtflib
DEPENDPATH += $$PWD/../dep-vtflib
