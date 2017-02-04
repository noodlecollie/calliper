#include "demoglwindow.h"
#include "model/shaders/simplelitshader.h"
#include "model/shaders/unlitpervertexcolorshader.h"
#include "renderer/opengl/openglhelpers.h"
#include "renderer/opengl/openglerrors.h"
#include "model/factories/genericbrushfactory.h"
#include "VTFLib/src/VTFFile.h"

class SimpleClassifier : public Model::IRenderPassClassifier
{
public:
    virtual int classify(quint32) const
    {
        return 0;
    }
};

DemoGLWindow::DemoGLWindow() : UserInterface::MapViewWindow(new SimpleClassifier())
{

}

DemoGLWindow::~DemoGLWindow()
{

}

void DemoGLWindow::initShaders()
{
    Model::ShaderPalette& palette = shaderPalette();
    palette.addItem(Model::ShaderPalette::DefaultShader, shaderStore()->addShaderProgram<Model::SimpleLitShader>());
    palette.addItem(Model::ShaderPalette::UnlitPerVertexColor, shaderStore()->addShaderProgram<Model::UnlitPerVertexColorShader>());
}

void DemoGLWindow::initTextures()
{
    textureStore()->setDefaultTextureFromFile(":model/textures/_ERROR_");

    {
        VTFLib::CVTFFile vtfFile;
        Q_ASSERT( vtfFile.Load("/Users/vesper/Documents/calliper/src/renderer-sandbox/rockground003.vtf") );

        Renderer::OpenGLTexturePointer texture = textureStore()->createEmptyTexture("test");
        texture->setSize(vtfFile.GetWidth(), vtfFile.GetHeight());

        // Possible variants:
        // QOpenGLTexture::RGB_DXT1
        // QOpenGLTexture::RGBA_DXT1
        // QOpenGLTexture::SRGB_DXT1
        // QOpenGLTexture::SRGB_Alpha_DXT1
        texture->setFormat(QOpenGLTexture::RGB_DXT1);

        texture->allocateStorage(QOpenGLTexture::RGB, QOpenGLTexture::UInt32);
        texture->setCompressedData(VTFLib::CVTFFile::ComputeImageSize(vtfFile.GetWidth(), vtfFile.GetHeight(),
                                                                      1, 1, vtfFile.GetFormat()),
                                   vtfFile.GetData(0,0,0,0));
        texture->create();
    }

    /////////////////////////////////////////////////////////////

    {
        VTFLib::CVTFFile vtfFile;
        Q_ASSERT( vtfFile.Load("/Users/vesper/Documents/calliper/src/renderer-sandbox/rockground003.vtf") );

        Renderer::OpenGLTexturePointer texture = textureStore()->createEmptyTexture("test2");
        GLTRY(texture->setSize(vtfFile.GetWidth(), vtfFile.GetHeight()));
        GLTRY(texture->setFormat(QOpenGLTexture::RGBA8U));

        GLTRY(texture->allocateStorage(QOpenGLTexture::RGBA, QOpenGLTexture::UInt32));
        GLTRY(texture->setData(QOpenGLTexture::RGBA, QOpenGLTexture::UInt32, vtfFile.GetData(0,0,0,0)));
        GLTRY(texture->create());
    }
}

void DemoGLWindow::initMaterials()
{
    Renderer::RenderMaterialPointer material = materialStore()->createMaterial("test");
    material->addTexture(Renderer::ShaderDefs::MainTexture, textureStore()->getTextureId("test"));
}

void DemoGLWindow::initLocalOpenGlSettings()
{
    using namespace Renderer;
    GL_CURRENT_F;

    GLTRY(f->glEnable(GL_CULL_FACE));
    GLTRY(f->glCullFace(GL_BACK));

    GLTRY(f->glEnable(GL_DEPTH_TEST));
    GLTRY(f->glDepthFunc(GL_LESS));
}

void DemoGLWindow::initializeGL()
{
    UserInterface::MapViewWindow::initializeGL();

    Model::GenericBrush* b =Model::GenericBrushFactory::createBrushFromMinMaxVectors(scene()->rootObject(),
                                                                              QVector3D(0,0,0),
                                                                              QVector3D(1024,1024,64));

    quint32 materialId = materialStore()->getMaterialId("test2");

    for ( int i = 0; i < b->brushFaceCount(); i++ )
    {
        Model::GenericBrushFace* f = b->brushFaceAt(i);
        f->texturePlane()->setMaterialId(materialId);
    }
}
