#-------------------------------------------------
#
# Project created by QtCreator 2017-04-26T12:44:58
#
#-------------------------------------------------

QT       += opengl

QT       -= gui

TARGET = rendersystem
TEMPLATE = lib

DEFINES += RENDERSYSTEM_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += rendersystem/global/rendersystem.cpp \
    rendersystem_global.cpp \
    rendersystem/interface-classes/currentcontextguard/contextreferencecounter.cpp \
    rendersystem/private/stores/opengltexturestore/opengltexturestore.cpp \
    rendersystem/private/shaders/common/vertexformat.cpp \
    rendersystem/private/opengl/opengluniformbuffer.cpp \
    rendersystem/private/opengl/openglvertexarrayobject.cpp \
    rendersystem/private/shaders/base/openglshaderprogram.cpp \
    rendersystem/private/stores/openglshaderstore/openglshaderstore.cpp \
    rendersystem/interface-classes/texture/namedopengltexture.cpp \
    rendersystem/private/shaders/common/globalshaderuniforms.cpp \
    rendersystem/private/shaders/derived/simplelitshader.cpp \
    rendersystem/private/shaders/derived/unlitpervertexcolorshader.cpp \
    rendersystem/interface-classes/rendermaterial/rendermaterial.cpp \
    rendersystem/private/shaders/common/privateshaderdefs.cpp \
    rendersystem/private/stores/materialstore/materialstore.cpp \
    rendersystem/interface-classes/geometry/geometrysection.cpp \
    rendersystem/interface-classes/geometry/geometrybuilder.cpp \
    rendersystem/endpoints/texturestoreendpoint.cpp \
    rendersystem/endpoints/materialstoreendpoint.cpp \
    rendersystem/private/rendermodel/rendermodel.cpp \
    rendersystem/private/rendermodel/rendergroup.cpp \
    rendersystem/private/rendermodel/partition/geometrydata.cpp \
    rendersystem/private/rendermodel/partition/geometrydatakey.cpp \
    rendersystem/private/rendermodel/partition/renderpartition.cpp \
    rendersystem/private/rendermodel/partition/openglbuffercollection.cpp \
    rendersystem/private/rendermodel/partition/geometryuploader.cpp \
    rendersystem/private/rendermodel/partition/geometrydatacontainer.cpp \
    rendersystem/private/rendermodel/partition/geometryconsolidator.cpp \
    rendersystem/private/rendermodel/partition/geometryoffsettable.cpp \
    rendersystem/private/rendermodel/partition/geometryrenderer.cpp \
    rendersystem/private/rendermode/baserendermode.cpp \
    rendersystem/private/rendermode/barebonesrendermode.cpp \
    rendersystem/private/stores/rendermodestore/rendermodestore.cpp \
    rendersystem/private/rendermodel/rendermodelcontext.cpp \
    rendersystem/private/rendermodel/partition/renderutils.cpp \
    rendersystem/interface-classes/definitions/materialdefs.cpp \
    rendersystem/interface-classes/definitions/texturedefs.cpp \
    rendersystem/interface-classes/definitions/rendermodeldefs.cpp \
    rendersystem/private/stores/rendermodelstore/rendermodelstore.cpp \
    rendersystem/endpoints/rendermodelstoreendpoint.cpp \
    rendersystem/interface-classes/definitions/framebufferdefs.cpp \
    rendersystem/private/stores/framebufferstore.h/framebufferstore.cpp \
    rendersystem/endpoints/framebufferstoreendpoint.cpp

HEADERS += rendersystem/global/rendersystem.h\
        rendersystem_global.h \
    rendersystem/private/store-classes/objectstore.h \
    rendersystem/interface-classes/currentcontextguard/currentcontextguard.h \
    rendersystem/interface-classes/currentcontextguard/contextreferencecounter.h \
    rendersystem/private/store-classes/objectstoreitempointer.h \
    rendersystem/private/stores/opengltexturestore/opengltexturestore.h \
    rendersystem/interfaces/itexturestore.h \
    rendersystem/private/shaders/common/vertexformat.h \
    rendersystem/private/opengl/opengluniformbuffer.h \
    rendersystem/private/opengl/openglvertexarrayobject.h \
    rendersystem/private/shaders/base/openglshaderprogram.h \
    rendersystem/private/store-classes/objectstoreitem.h \
    rendersystem/private/store-classes/itembasedobjectstore.h \
    rendersystem/private/store-classes/itempointerbasedobjectstore.h \
    rendersystem/private/stores/openglshaderstore/openglshaderstore.h \
    rendersystem/interface-classes/texture/namedopengltexture.h \
    rendersystem/private/shaders/common/globalshaderuniforms.h \
    rendersystem/private/shaders/derived/simplelitshader.h \
    rendersystem/private/store-classes/staticobjectstore.h \
    rendersystem/private/shaders/derived/unlitpervertexcolorshader.h \
    rendersystem/interface-classes/rendermaterial/rendermaterial.h \
    rendersystem/private/shaders/common/privateshaderdefs.h \
    rendersystem/private/stores/materialstore/materialstore.h \
    rendersystem/interface-classes/geometry/geometrysection.h \
    rendersystem/interface-classes/geometry/geometrybuilder.h \
    rendersystem/endpoints/texturestoreendpoint.h \
    rendersystem/interfaces/imaterialstore.h \
    rendersystem/private/store-classes/pathmanagingobjectstore.h \
    rendersystem/endpoints/materialstoreendpoint.h \
    rendersystem/private/rendermodel/rendermodel.h \
    rendersystem/private/rendermodel/rendergroup.h \
    rendersystem/private/rendermodel/partition/geometrydata.h \
    rendersystem/private/rendermodel/partition/geometrydatakey.h \
    rendersystem/private/rendermodel/partition/renderpartition.h \
    rendersystem/private/rendermodel/partition/openglbuffercollection.h \
    rendersystem/private/rendermodel/partition/geometryuploader.h \
    rendersystem/private/rendermodel/partition/geometrydatacontainer.h \
    rendersystem/private/store-classes/globalinstancehelper.h \
    rendersystem/private/rendermodel/partition/geometryconsolidator.h \
    rendersystem/private/rendermodel/partition/geometryoffsettable.h \
    rendersystem/private/rendermodel/partition/geometryrenderer.h \
    rendersystem/private/rendermodel/rendermodel.h \
    rendersystem/private/rendermode/baserendermode.h \
    rendersystem/private/rendermode/barebonesrendermode.h \
    rendersystem/private/stores/rendermodestore/rendermodestore.h \
    rendersystem/private/rendermodel/rendermodelcontext.h \
    rendersystem/private/store-classes/staticobjectstorekey.h \
    rendersystem/private/stores/openglshaderstore/openglshaderstorekey.h \
    rendersystem/private/stores/rendermodestore/rendermodestorekey.h \
    rendersystem/private/rendermodel/partition/renderutils.h \
    rendersystem/interface-classes/definitions/materialdefs.h \
    rendersystem/interface-classes/definitions/texturedefs.h \
    rendersystem/interface-classes/definitions/rendermodeldefs.h \
    rendersystem/interface-classes/definitions/shaderdefs.h \
    rendersystem/interfaces/irendermodelstore.h \
    rendersystem/private/stores/rendermodelstore/rendermodelstore.h \
    rendersystem/endpoints/rendermodelstoreendpoint.h \
    rendersystem/interfaces/iframebufferstore.h \
    rendersystem/interface-classes/definitions/framebufferdefs.h \
    rendersystem/private/stores/framebufferstore.h/framebufferstore.h \
    rendersystem/endpoints/framebufferstoreendpoint.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

RESOURCES += \
    rendersystem/private/resource/resource.qrc

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../calliperutil/release/ -lcalliperutil
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../calliperutil/debug/ -lcalliperutil
else:unix: LIBS += -L$$OUT_PWD/../calliperutil/ -lcalliperutil

INCLUDEPATH += $$PWD/../calliperutil
DEPENDPATH += $$PWD/../calliperutil
