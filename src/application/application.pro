#-------------------------------------------------
#
# Project created by QtCreator 2015-09-27T21:39:33
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = calliper
TEMPLATE = app

INCLUDEPATH += \
    application \
    factories \
    geometry \
    math \
    rendering \
    scene \
    serialisation \
    shaders \
    tools

HEADERS += \
    application/application.h \
    application/mainwindow.h \
    application/mapdocument.h \
    application/resourcemanager.h \
    application/simplenumericfont.h \
    factories/brushfactory.h \
    factories/factories.h \
    factories/geometryfactory.h \
    factories/scenefactory.h \
    geometry/boundingbox.h \
    geometry/geometrydata.h \
    math/callipermath.h \
    math/eulerangle.h \
    math/generalutil.h \
    math/ray3d.h \
    rendering/cameralens.h \
    rendering/cameraparams.h \
    rendering/matrixstack.h \
    rendering/openglrenderer.h \
    rendering/shaderstack.h \
    rendering/viewport.h \
    rendering/viewportuseroptions.h \
    scene/brush.h \
    scene/brushface.h \
    scene/cameracontroller.h \
    scene/hierarchicalobject.h \
    scene/mapgrid.h \
    scene/mapscene.h \
    scene/originmarker.h \
    scene/scenecamera.h \
    scene/sceneobject.h \
    scene/textureplane.h \
    scene/translationhandle.h \
    scene/uimanipulator.h \
    scene/uiscene.h \
    serialisation/callipermapfile.h \
    serialisation/cmfoptionsdialog.h \
    serialisation/iserialisable.h \
    serialisation/jsonutil.h \
    serialisation/objfileparser.h \
    scene/basescene.h \
    shaders/basiclittextureshader.h \
    shaders/minimumshader.h \
    shaders/minimumtexturedshader.h \
    shaders/pervertexcolorshader.h \
    shaders/selectionmaskshader.h \
    shaders/shaderprogram.h \
    shaders/shaders.h \
    shaders/unlittextureshader.h \
    tools/basetool.h \
    tools/debugtesttool.h \
    tools/inputprocessor.h \
    tools/sceneobjectmanipulator.h \
    tools/tools.h \
    tools/translationtool.h \
    tools/scaletool.h \
    scene/scalehandle.h \
    scene/blockcreationhandle.h \
    math/iraydetectable.h \
    tools/uimanipulatortool.h \
    scene/spatialsnapshot.h \
    tools/creategeometrytool.h \
    factories/sceneobjectfactory.h \
    math/raytracecontact.h \
    scene/crosshair3d.h \
    scene/basedraghandle.h \
    scene/uiblockcreationhandle.h \
    scene/axisdraghandle.h \
    serialisation/keyvaluesparser.h \
    serialisation/keyvaluestoken.h \
    math/plane3d.h \
    geometry/winding3d.h \
    geometry/texturedpolygon.h \
    geometry/geometryutil.h \
    geometry/windingvertex.h \
    geometry/fuzzyvertexmap.h \
    serialisation/valvemapfile.h

SOURCES += \
    application/application.cpp \
    application/mainwindow.cpp \
    application/mapdocument.cpp \
    application/resourcemanager.cpp \
    application/simplenumericfont.cpp \
    factories/brushfactory.cpp \
    factories/geometryfactory.cpp \
    factories/scenefactory.cpp \
    geometry/boundingbox.cpp \
    geometry/geometrydata.cpp \
    math/callipermath.cpp \
    math/eulerangle.cpp \
    math/generalutil.cpp \
    math/ray3d.cpp \
    rendering/cameralens.cpp \
    rendering/cameraparams.cpp \
    rendering/matrixstack.cpp \
    rendering/openglrenderer.cpp \
    rendering/shaderstack.cpp \
    rendering/viewport.cpp \
    rendering/viewportuseroptions.cpp \
    scene/brush.cpp \
    scene/brushface.cpp \
    scene/cameracontroller.cpp \
    scene/hierarchicalobject.cpp \
    scene/mapgrid.cpp \
    scene/mapscene.cpp \
    scene/originmarker.cpp \
    scene/scenecamera.cpp \
    scene/sceneobject.cpp \
    scene/textureplane.cpp \
    scene/translationhandle.cpp \
    scene/uimanipulator.cpp \
    scene/uiscene.cpp \
    serialisation/callipermapfile.cpp \
    serialisation/cmfoptionsdialog.cpp \
    serialisation/jsonutil.cpp \
    serialisation/objfileparser.cpp \
    scene/basescene.cpp \
    shaders/basiclittextureshader.cpp \
    shaders/minimumshader.cpp \
    shaders/minimumtexturedshader.cpp \
    shaders/pervertexcolorshader.cpp \
    shaders/selectionmaskshader.cpp \
    shaders/shaderprogram.cpp \
    shaders/unlittextureshader.cpp \
    tools/basetool.cpp \
    tools/debugtesttool.cpp \
    tools/inputprocessor.cpp \
    tools/sceneobjectmanipulator.cpp \
    tools/translationtool.cpp \
    application/main.cpp \
    tools/scaletool.cpp \
    scene/scalehandle.cpp \
    scene/blockcreationhandle.cpp \
    tools/uimanipulatortool.cpp \
    scene/spatialsnapshot.cpp \
    tools/creategeometrytool.cpp \
    factories/sceneobjectfactory.cpp \
    scene/crosshair3d.cpp \
    scene/basedraghandle.cpp \
    scene/uiblockcreationhandle.cpp \
    scene/axisdraghandle.cpp \
    serialisation/keyvaluesparser.cpp \
    serialisation/keyvaluestoken.cpp \
    math/plane3d.cpp \
    geometry/winding3d.cpp \
    geometry/texturedpolygon.cpp \
    geometry/geometryutil.cpp \
    geometry/windingvertex.cpp \
    geometry/fuzzyvertexmap.cpp \
    serialisation/valvemapfile.cpp

RESOURCES += \
    resource/resource.qrc \
    qdarkstyle/style.qrc

FORMS += \
    application/mainwindow.ui \
    rendering/viewportuseroptions.ui \
    serialisation/cmfoptionsdialog.ui
