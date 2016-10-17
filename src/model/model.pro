#-------------------------------------------------
#
# Project created by QtCreator 2016-09-03T22:28:04
#
#-------------------------------------------------

QT       += opengl

TARGET = model
TEMPLATE = lib

DEFINES += MODEL_LIBRARY

SOURCES += \
    stores/shaderstore.cpp \
    stores/texturestore.cpp \
    scene/scene.cpp \
    scene/sceneobject.cpp \
    math/eulerangle.cpp \
    events/spatialconfigurationchange.cpp \
    scene/hierarchystate.cpp \
    factories/geometryfactory.cpp \
    sceneobjects/debugcube.cpp \
    math/modelmath.cpp \
    camera/scenecamera.cpp \
    scenerenderer/scenerenderer.cpp \
    camera/cameralens.cpp \
    scene/sceneobjectinitparams.cpp \
    genericbrush/genericbrush.cpp \
    genericbrush/genericbrushface.cpp \
    genericbrush/textureplane.cpp \
    camera/cameracontroller.cpp \
    keymap/keysignalsender.cpp \
    keymap/keymap.cpp

HEADERS += \
    stores/shaderstore.h \
    stores/texturestore.h \
    scene/scene.h \
    scene/sceneobject.h \
    math/eulerangle.h \
    events/spatialconfigurationchange.h \
    events/modeleventtypes.h \
    scene/hierarchystate.h \
    factories/geometryfactory.h \
    sceneobjects/debugcube.h \
    math/modelmath.h \
    camera/scenecamera.h \
    scenerenderer/scenerenderer.h \
    camera/cameralens.h \
    scenerenderer/irenderpassclassifier.h \
    scene/sceneobjectinitparams.h \
    genericbrush/genericbrush.h \
    genericbrush/genericbrushface.h \
    genericbrush/textureplane.h \
    camera/cameracontroller.h \
    keymap/keysignalsender.h \
    keymap/keymap.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

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
