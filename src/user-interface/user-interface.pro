#-------------------------------------------------
#
# Project created by QtCreator 2016-12-05T18:52:01
#
#-------------------------------------------------

QT += opengl

TARGET = user-interface
TEMPLATE = lib

DEFINES += USERINTERFACE_LIBRARY

SOURCES += \
    user-interface_global.cpp \
    user-interface/arrangeable-tabs/model/quadgridlayoutanalyser.cpp \
    user-interface/arrangeable-tabs/model/quadgridlayoutdefs.cpp \
    user-interface/arrangeable-tabs/model/quadgridlayoutmodel.cpp \
    user-interface/arrangeable-tabs/model/quadgridlayoutpoint.cpp \
    user-interface/arrangeable-tabs/resizablegridwrapper.cpp \
    user-interface/arrangeable-tabs/resizeablegridelementbutton.cpp \
    user-interface/arrangeable-tabs/resizeablegridlayoutcontainer.cpp \
    user-interface/arrangeable-tabs/resizeablegridlayoutcontainerhandle.cpp \
    user-interface/arrangeable-tabs/resizeablegridlayoutmanager.cpp \
    user-interface/views/mapviewwindow.cpp \
    user-interface/arrangeable-tabs/container/resizablegridlayoutcontainerbutton.cpp

HEADERS +=\
    user-interface_global.h \
    user-interface/arrangeable-tabs/model/quadgridlayoutanalyser.h \
    user-interface/arrangeable-tabs/model/quadgridlayoutdefs.h \
    user-interface/arrangeable-tabs/model/quadgridlayoutmodel.h \
    user-interface/arrangeable-tabs/model/quadgridlayoutpoint.h \
    user-interface/arrangeable-tabs/resizablegridwrapper.h \
    user-interface/arrangeable-tabs/resizeablegridelementbutton.h \
    user-interface/arrangeable-tabs/resizeablegridlayoutcontainer.h \
    user-interface/arrangeable-tabs/resizeablegridlayoutcontainerhandle.h \
    user-interface/arrangeable-tabs/resizeablegridlayoutmanager.h \
    user-interface/views/mapviewwindow.h \
    user-interface/arrangeable-tabs/container/resizablegridlayoutcontainerbutton.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../calliperutil/release/ -lcalliperutil
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../calliperutil/debug/ -lcalliperutil
else:unix: LIBS += -L$$OUT_PWD/../calliperutil/ -lcalliperutil

INCLUDEPATH += $$PWD/../calliperutil
DEPENDPATH += $$PWD/../calliperutil

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../dep-vtflib/release/ -ldep-vtflib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../dep-vtflib/debug/ -ldep-vtflib
else:unix: LIBS += -L$$OUT_PWD/../dep-vtflib/ -ldep-vtflib

INCLUDEPATH += $$PWD/../dep-vtflib
DEPENDPATH += $$PWD/../dep-vtflib

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../model-loaders/release/ -lmodel-loaders
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../model-loaders/debug/ -lmodel-loaders
else:unix: LIBS += -L$$OUT_PWD/../model-loaders/ -lmodel-loaders

INCLUDEPATH += $$PWD/../model-loaders
DEPENDPATH += $$PWD/../model-loaders

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../file-formats/release/ -lfile-formats
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../file-formats/debug/ -lfile-formats
else:unix: LIBS += -L$$OUT_PWD/../file-formats/ -lfile-formats

INCLUDEPATH += $$PWD/../file-formats
DEPENDPATH += $$PWD/../file-formats

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

FORMS +=
