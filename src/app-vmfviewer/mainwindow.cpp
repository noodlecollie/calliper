#include "mainwindow.h"
#include "model/shaders/unlitshader.h"
#include "model/shaders/unlitpervertexcolorshader.h"
#include "renderer/opengl/openglerrors.h"
#include "renderer/opengl/openglhelpers.h"
#include <QtDebug>
#include "model/shaders/errorshader.h"
#include "model/genericbrush/genericbrush.h"
#include "model/shaders/simplelitshader.h"
#include <QMessageBox>
#include <QtGlobal>
#include <QMap>
#include "file-formats/vpk/vpkindextreerecord.h"
#include "model-loaders/vtf/vtfloader.h"

class SimpleRenderClassifier : public Model::IRenderPassClassifier
{
public:
    virtual int classify(quint32 objectId) const override
    {
        Q_UNUSED(objectId);
        return 0;
    }

};

MainWindow::MainWindow() : UserInterface::MapViewWindow(new SimpleRenderClassifier()),
    m_iPlaceholderMaterial(0)
{
    resize(640, 480);
}

MainWindow::~MainWindow()
{
}

void MainWindow::initShaders()
{
    Model::ShaderPalette& palette = shaderPalette();
    Model::ShaderStore* shaderStore = Model::ResourceEnvironment::globalInstance()->shaderStore();
    palette.addItem(Model::ShaderPalette::DefaultShader, shaderStore->addShaderProgram<Model::SimpleLitShader>());
    palette.addItem(Model::ShaderPalette::UnlitPerVertexColor, shaderStore->addShaderProgram<Model::UnlitPerVertexColorShader>());
}

void MainWindow::initTextures()
{
    Model::TextureStore* textureStore = Model::ResourceEnvironment::globalInstance()->textureStore();
    textureStore->setDefaultTextureFromFile(":model/textures/_ERROR_");
    textureStore->createTextureFromFile(":model/textures/dev/devwhite");
}

void MainWindow::initMaterials()
{
    Model::MaterialStore* materialStore = Model::ResourceEnvironment::globalInstance()->materialStore();
    Model::TextureStore* textureStore = Model::ResourceEnvironment::globalInstance()->textureStore();

    // For now the string must be identical.
    Renderer::RenderMaterialPointer material = materialStore->createMaterial(":model/textures/dev/devwhite");
    m_iPlaceholderMaterial = material->materialStoreId();

    material->addTexture(Renderer::ShaderDefs::MainTexture, textureStore->getTextureId(":model/textures/dev/devwhite"));
}

void MainWindow::initLocalOpenGlSettings()
{
    using namespace Renderer;
    GL_CURRENT_F;

    GLTRY(f->glEnable(GL_CULL_FACE));
    GLTRY(f->glCullFace(GL_BACK));

    GLTRY(f->glEnable(GL_DEPTH_TEST));
    GLTRY(f->glDepthFunc(GL_LESS));
}

void MainWindow::processBrushes()
{
    using namespace Model;

    QList<GenericBrush*> brushes = mapDataModel()->scene()->rootObject()->findChildren<GenericBrush*>();

    foreach ( GenericBrush* brush, brushes )
    {
        float rand = (float)qrand()/(float)RAND_MAX;
        rand = 0.5f + (0.5f * rand);
        unsigned char randChar = (unsigned char)(rand * 0xff);
        QRgb col = (0xff << 24) | (randChar << 16) | (randChar << 8) | randChar;
        brush->setColor(QColor::fromRgb(col));

        for ( int i = 0; i < brush->brushFaceCount(); i++ )
        {
            GenericBrushFace* face = brush->brushFaceAt(i);
            face->texturePlane()->setMaterialId(m_iPlaceholderMaterial);
        }
    }
}

void MainWindow::importTextures()
{
    ModelLoaders::VTFLoader loader(Model::ResourceEnvironment::globalInstance()->materialStore(),
                                   Model::ResourceEnvironment::globalInstance()->textureStore());
    loadVpks();
    loader.loadMaterials(vpkFileCollection());
}
