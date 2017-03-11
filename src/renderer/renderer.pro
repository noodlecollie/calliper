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
    renderer_global.cpp \
    renderer/geometry/geometrybuilder.cpp \
    renderer/geometry/geometrysection.cpp \
    renderer/geometry/vertex3d.cpp \
    renderer/materials/rendermaterial.cpp \
    renderer/opengl/openglerrors.cpp \
    renderer/opengl/openglshaderprogram.cpp \
    renderer/opengl/opengltexture.cpp \
    renderer/opengl/opengluniformbuffer.cpp \
    renderer/opengl/openglvertexarrayobject.cpp \
    renderer/rendermodel/0-modellevel/rendermodel.cpp \
    renderer/rendermodel/0-modellevel/rendermodelkey.cpp \
    renderer/rendermodel/1-passlevel/rendermodelpass.cpp \
    renderer/rendermodel/1-passlevel/rendermodelpasskey.cpp \
    renderer/rendermodel/2-batchgrouplevel/rendermodelbatchgroup.cpp \
    renderer/rendermodel/2-batchgrouplevel/rendermodelbatchgroupkey.cpp \
    renderer/rendermodel/3-batchlevel/matrixbatch.cpp \
    renderer/rendermodel/3-batchlevel/matrixbatchkey.cpp \
    renderer/rendermodel/3-batchlevel/openglbatch.cpp \
    renderer/rendermodel/4-batchitemlevel/matrixbatchitem.cpp \
    renderer/rendermodel/4-batchitemlevel/matrixbatchitemkey.cpp \
    renderer/rendermodel/rendererdrawparams.cpp \
    renderer/rendermodel/rendererinputobjectparams.cpp \
    renderer/shaders/globalshaderuniforms.cpp \
    renderer/shaders/shaderdefs.cpp \
    renderer/shaders/vertexformat.cpp \
    renderer/global/openglbackgroundcontext.cpp \
    renderer/shaders/baseshaderpalette.cpp

HEADERS +=\
        renderer_global.h \
    renderer/functors/imaterialretrievalfunctor.h \
    renderer/functors/ishaderretrievalfunctor.h \
    renderer/functors/itextureretrievalfunctor.h \
    renderer/geometry/geometrybuilder.h \
    renderer/geometry/geometrysection.h \
    renderer/geometry/vertex3d.h \
    renderer/materials/rendermaterial.h \
    renderer/opengl/openglerrors.h \
    renderer/opengl/openglhelpers.h \
    renderer/opengl/openglshaderprogram.h \
    renderer/opengl/opengltexture.h \
    renderer/opengl/opengluniformbuffer.h \
    renderer/opengl/openglvertexarrayobject.h \
    renderer/rendermodel/0-modellevel/rendermodel.h \
    renderer/rendermodel/0-modellevel/rendermodelkey.h \
    renderer/rendermodel/1-passlevel/rendermodelpass.h \
    renderer/rendermodel/1-passlevel/rendermodelpasskey.h \
    renderer/rendermodel/2-batchgrouplevel/rendermodelbatchgroup.h \
    renderer/rendermodel/2-batchgrouplevel/rendermodelbatchgroupkey.h \
    renderer/rendermodel/3-batchlevel/matrixbatch.h \
    renderer/rendermodel/3-batchlevel/matrixbatchkey.h \
    renderer/rendermodel/3-batchlevel/openglbatch.h \
    renderer/rendermodel/4-batchitemlevel/matrixbatchitem.h \
    renderer/rendermodel/4-batchitemlevel/matrixbatchitemkey.h \
    renderer/rendermodel/rendererdrawparams.h \
    renderer/rendermodel/rendererinputobjectparams.h \
    renderer/rendermodel/rendererobjectflags.h \
    renderer/shaders/globalshaderuniforms.h \
    renderer/shaders/ishaderspec.h \
    renderer/shaders/shaderdefs.h \
    renderer/shaders/vertexformat.h \
    renderer/global/openglbackgroundcontext.h \
    renderer/shaders/baseshaderpalette.h \
    renderer/functors/renderfunctorgroup.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

RESOURCES += \
    renderer/resource/resource.qrc

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../calliperutil/release/ -lcalliperutil
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../calliperutil/debug/ -lcalliperutil
else:unix: LIBS += -L$$OUT_PWD/../calliperutil/ -lcalliperutil

INCLUDEPATH += $$PWD/../calliperutil
DEPENDPATH += $$PWD/../calliperutil
