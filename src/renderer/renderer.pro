#-------------------------------------------------
#
# Project created by QtCreator 2016-08-17T21:21:29
#
#-------------------------------------------------

QT       += opengl

TARGET = renderer
TEMPLATE = lib

DEFINES += RENDERER_LIBRARY

SOURCES += \
    opengl/openglerrors.cpp \
    opengl/opengluniformbuffer.cpp \
    shaders/shaderdefs.cpp \
    opengl/openglshaderprogram.cpp \
    shaders/debugscreenspaceshader.cpp \
    rendermodel/1-passlevel/rendermodelpass.cpp \
    opengl/opengltexture.cpp \
    geometry/vertex3d.cpp \
    geometry/geometrybuilder.cpp \
    shaders/vertexformat.cpp \
    geometry/geometrysection.cpp \
    rendermodel/1-passlevel/rendermodelpassattributes.cpp \
    rendermodel/0-modellevel/rendermodel.cpp \
    renderer.cpp \
    rendermodel/0-modellevel/rendermodelkey.cpp \
    rendermodel/4-batchitemlevel/matrixbatchitemkey.cpp \
    rendermodel/3-batchlevel/matrixbatchkey.cpp \
    rendermodel/1-passlevel/rendermodelpasskey.cpp \
    rendermodel/2-batchgrouplevel/rendermodelbatchgroupkey.cpp \
    rendermodel/4-batchitemlevel/matrixbatchitem.cpp \
    rendermodel/3-batchlevel/matrixbatch.cpp \
    rendermodel/3-batchlevel/openglbatch.cpp \
    rendermodel/2-batchgrouplevel/rendermodelbatchgroup.cpp

HEADERS +=\
        renderer_global.h \
    opengl/openglerrors.h \
    shaders/shaderdefs.h \
    opengl/openglhelpers.h \
    shaders/ishaderspec.h \
    opengl/opengluniformbuffer.h \
    opengl/openglshaderprogram.h \
    shaders/debugscreenspaceshader.h \
    rendermodel/1-passlevel/rendermodelpass.h \
    opengl/opengltexture.h \
    functors/ishaderretrievalfunctor.h \
    functors/itextureretrievalfunctor.h \
    geometry/vertex3d.h \
    geometry/geometrybuilder.h \
    shaders/vertexformat.h \
    geometry/geometrysection.h \
    rendermodel/1-passlevel/rendermodelpassattributes.h \
    rendermodel/0-modellevel/rendermodel.h \
    irenderer.h \
    rendermodel/0-modellevel/rendermodelkey.h \
    rendermodel/4-batchitemlevel/matrixbatchitemkey.h \
    rendermodel/3-batchlevel/matrixbatchkey.h \
    rendermodel/1-passlevel/rendermodelpasskey.h \
    rendermodel/2-batchgrouplevel/rendermodelbatchgroupkey.h \
    rendermodel/4-batchitemlevel/matrixbatchitem.h \
    rendermodel/3-batchlevel/matrixbatch.h \
    rendermodel/3-batchlevel/openglbatch.h \
    rendermodel/2-batchgrouplevel/rendermodelbatchgroup.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

RESOURCES += \
    resource/resource.qrc

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../calliperutil/release/ -lcalliperutil
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../calliperutil/debug/ -lcalliperutil
else:unix: LIBS += -L$$OUT_PWD/../calliperutil/ -lcalliperutil

INCLUDEPATH += $$PWD/../calliperutil
DEPENDPATH += $$PWD/../calliperutil
