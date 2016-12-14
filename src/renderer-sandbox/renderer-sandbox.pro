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
    demoglwindow.cpp

HEADERS  += \
    demoglwindow.h

FORMS    +=

RESOURCES += \
    resource.qrc

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../calliperutil/release/ -lcalliperutil
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../calliperutil/debug/ -lcalliperutil
else:unix: LIBS += -L$$OUT_PWD/../calliperutil/ -lcalliperutil

INCLUDEPATH += $$PWD/../calliperutil
DEPENDPATH += $$PWD/../calliperutil

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../high-level-convenience/release/ -lhigh-level-convenience
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../high-level-convenience/debug/ -lhigh-level-convenience
else:unix: LIBS += -L$$OUT_PWD/../high-level-convenience/ -lhigh-level-convenience

INCLUDEPATH += $$PWD/../high-level-convenience
DEPENDPATH += $$PWD/../high-level-convenience

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

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../dep-vtflib/release/ -ldep-vtflib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../dep-vtflib/debug/ -ldep-vtflib
else:unix: LIBS += -L$$OUT_PWD/../dep-vtflib/ -ldep-vtflib

INCLUDEPATH += $$PWD/../dep-vtflib
DEPENDPATH += $$PWD/../dep-vtflib

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
