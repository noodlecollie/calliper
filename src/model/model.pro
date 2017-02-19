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
    model_global.cpp \
    model/camera/cameracontroller.cpp \
    model/camera/cameralens.cpp \
    model/camera/scenecamera.cpp \
    model/controller-adapters/keymap.cpp \
    model/controller-adapters/keysignalsender.cpp \
    model/controller-adapters/mouseeventmap.cpp \
    model/core/datachangenotifier.cpp \
    model/events/spatialconfigurationchange.cpp \
    model/factories/genericbrushfactory.cpp \
    model/factories/geometryfactory.cpp \
    model/genericbrush/genericbrush.cpp \
    model/genericbrush/genericbrushface.cpp \
    model/genericbrush/textureplane.cpp \
    model/math/eulerangle.cpp \
    model/math/fuzzyvertexmap.cpp \
    model/math/modelmath.cpp \
    model/math/plane3d.cpp \
    model/math/ray3d.cpp \
    model/math/texturedwinding.cpp \
    model/math/winding3d.cpp \
    model/math/windingvertex.cpp \
    model/projects/calliperproject.cpp \
    model/projects/calliperprojectmetadata.cpp \
    model/scene/hierarchystate.cpp \
    model/scene/scene.cpp \
    model/scene/sceneobject.cpp \
    model/scene/sceneobjectinitparams.cpp \
    model/sceneobjects/debugcube.cpp \
    model/sceneobjects/debugtriangle.cpp \
    model/sceneobjects/originmarker.cpp \
    model/scenerenderer/scenerenderer.cpp \
    model/shaders/errorshader.cpp \
    model/shaders/shaderpalette.cpp \
    model/shaders/simplelitshader.cpp \
    model/shaders/unlitpervertexcolorshader.cpp \
    model/shaders/unlitshader.cpp \
    model/stores/materialstore.cpp \
    model/stores/shaderstore.cpp \
    model/stores/texturestore.cpp \
    model/global/resourceenvironment.cpp \
    model/scene/mapscene.cpp

HEADERS += \
    model_global.h \
    model/camera/cameracontroller.h \
    model/camera/cameralens.h \
    model/camera/scenecamera.h \
    model/controller-adapters/keymap.h \
    model/controller-adapters/keysignalsender.h \
    model/controller-adapters/mouseeventmap.h \
    model/core/datachangenotifier.h \
    model/events/modeleventtypes.h \
    model/events/spatialconfigurationchange.h \
    model/factories/genericbrushfactory.h \
    model/factories/geometryfactory.h \
    model/genericbrush/genericbrush.h \
    model/genericbrush/genericbrushface.h \
    model/genericbrush/textureplane.h \
    model/math/eulerangle.h \
    model/math/fuzzyvertexmap.h \
    model/math/modelmath.h \
    model/math/plane3d.h \
    model/math/ray3d.h \
    model/math/texturedwinding.h \
    model/math/winding3d.h \
    model/math/windingvertex.h \
    model/projects/calliperproject.h \
    model/projects/calliperprojectmetadata.h \
    model/scene/hierarchystate.h \
    model/scene/scene.h \
    model/scene/sceneobject.h \
    model/scene/sceneobjectinitparams.h \
    model/sceneobjects/debugcube.h \
    model/sceneobjects/debugtriangle.h \
    model/sceneobjects/originmarker.h \
    model/scenerenderer/irenderpassclassifier.h \
    model/scenerenderer/scenerenderer.h \
    model/shaders/errorshader.h \
    model/shaders/shaderpalette.h \
    model/shaders/simplelitshader.h \
    model/shaders/unlitpervertexcolorshader.h \
    model/shaders/unlitshader.h \
    model/stores/materialstore.h \
    model/stores/shaderstore.h \
    model/stores/texturestore.h \
    model/global/resourceenvironment.h \
    model/scene/mapscene.h

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

RESOURCES += \
    model/resource/resource.qrc
