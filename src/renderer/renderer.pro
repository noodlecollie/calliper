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
    rendermodel/3-batchlevel/rendermodelbatch.cpp \
    rendermodel/3-batchlevel/rendermodelbatchparams.cpp \
    rendermodel/4-batchitemlevel/rendermodelbatchitem.cpp \
    opengl/opengluniformbuffer.cpp \
    shaders/shaderdefs.cpp \
    opengl/openglshaderprogram.cpp \
    rendermodel/3-batchlevel/rendermodelbatchkey.cpp \
    shaders/debugscreenspaceshader.cpp \
    rendermodel/1-passlevel/rendermodelpass.cpp \
    opengl/opengltexture.cpp \
    geometry/vertex3d.cpp \
    geometry/geometrybuilder.cpp \
    shaders/vertexformat.cpp \
    geometry/geometrysection.cpp \
    rendermodel/1-passlevel/rendermodelpassattributes.cpp \
    rendermodel/4-batchitemlevel/rendermodelbatchitemkey.cpp \
    rendermodel/0-modellevel/rendermodel.cpp \
    renderer.cpp \
    rendermodel/0-modellevel/rendermodelinputparams.cpp

HEADERS +=\
        renderer_global.h \
    opengl/openglerrors.h \
    rendermodel/3-batchlevel/rendermodelbatch.h \
    shaders/shaderdefs.h \
    rendermodel/3-batchlevel/rendermodelbatchparams.h \
    opengl/openglhelpers.h \
    rendermodel/4-batchitemlevel/rendermodelbatchitem.h \
    shaders/ishaderspec.h \
    opengl/opengluniformbuffer.h \
    opengl/openglshaderprogram.h \
    rendermodel/3-batchlevel/rendermodelbatchkey.h \
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
    rendermodel/4-batchitemlevel/rendermodelbatchitemkey.h \
    rendermodel/0-modellevel/rendermodel.h \
    irenderer.h \
    rendermodel/0-modellevel/rendermodelinputparams.h

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
