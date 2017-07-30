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
    user-interface/arrangeable-tabs/grid/resizeablegridelementbutton.cpp \
    user-interface/arrangeable-tabs/grid/resizeablegridlayoutmanager.cpp \
    user-interface/views/mapviewwindow.cpp \
    user-interface/arrangeable-tabs/container/resizeablegridlayoutcontainer.cpp \
    user-interface/arrangeable-tabs/container/resizeablegridlayoutcontainerbutton.cpp \
    user-interface/arrangeable-tabs/container/resizeablegridlayoutcontainermenu.cpp \
    user-interface/arrangeable-tabs/widget/quadgridwidget.cpp \
    user-interface/modelviews/modelviewfactory.cpp \
    user-interface/views/mapviewport.cpp \
    user-interface/icons/filedatamodeliconprovider.cpp \
    user-interface/opengl/framebuffercopier.cpp \
    user-interface/widgets/mainwindowslavewidget.cpp \
    user-interface/signal-handlers/visibilityactionhandler.cpp \
    user-interface/widgets/visibility-action/visibilityactiondockwidget.cpp \
    user-interface/widgets/visibility-action/visibilityactionwidget.cpp

HEADERS +=\
    user-interface_global.h \
    user-interface/arrangeable-tabs/model/quadgridlayoutanalyser.h \
    user-interface/arrangeable-tabs/model/quadgridlayoutdefs.h \
    user-interface/arrangeable-tabs/model/quadgridlayoutmodel.h \
    user-interface/arrangeable-tabs/model/quadgridlayoutpoint.h \
    user-interface/arrangeable-tabs/grid/resizeablegridelementbutton.h \
    user-interface/arrangeable-tabs/grid/resizeablegridlayoutmanager.h \
    user-interface/views/mapviewwindow.h \
    user-interface/arrangeable-tabs/container/resizeablegridlayoutcontainer.h \
    user-interface/arrangeable-tabs/container/resizeablegridlayoutcontainermenu.h \
    user-interface/arrangeable-tabs/container/resizeablegridlayoutcontainerbutton.h \
    user-interface/arrangeable-tabs/widget/quadgridwidget.h \
    user-interface/modelviews/modelviewfactory.h \
    user-interface/views/mapviewport.h \
    user-interface/icons/filedatamodeliconprovider.h \
    user-interface/modelviews/imodelview.h \
    user-interface/opengl/framebuffercopier.h \
    user-interface/widgets/mainwindowslavewidget.h \
    user-interface/signal-handlers/visibilityactionhandler.h \
    user-interface/widgets/visibility-action/visibilityactiondockwidget.h \
    user-interface/widgets/visibility-action/visibilityactionwidget.h

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

FORMS +=

RESOURCES += \
    user-interface/resource/resource.qrc

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../rendersystem/release/ -lrendersystem
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../rendersystem/debug/ -lrendersystem
else:unix: LIBS += -L$$OUT_PWD/../rendersystem/ -lrendersystem

INCLUDEPATH += $$PWD/../rendersystem
DEPENDPATH += $$PWD/../rendersystem
