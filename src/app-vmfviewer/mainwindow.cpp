#include "mainwindow.h"
#include "shaders/unlitshader.h"
#include "shaders/unlitpervertexcolorshader.h"
#include "opengl/openglerrors.h"
#include "opengl/openglhelpers.h"
#include <QtDebug>
#include "shaders/errorshader.h"
#include "genericbrush/genericbrush.h"
#include "shaders/simplelitshader.h"
#include <QMessageBox>
#include <QtGlobal>
#include <QMap>
#include "vpk/vpkindextreerecord.h"
#include "vtf/vtfloader.h"

class SimpleRenderClassifier : public Model::IRenderPassClassifier
{
public:
    virtual int classify(quint32 objectId) const override
    {
        Q_UNUSED(objectId);
        return 0;
    }

};

MainWindow::MainWindow() : HighLevelConvenience::MapViewWindow(new SimpleRenderClassifier()),
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
    palette.addItem(Model::ShaderPalette::DefaultShader, shaderStore()->addShaderProgram<Model::SimpleLitShader>());
    palette.addItem(Model::ShaderPalette::UnlitPerVertexColor, shaderStore()->addShaderProgram<Model::UnlitPerVertexColorShader>());
}

void MainWindow::initTextures()
{
    textureStore()->setDefaultTextureFromFile(":model/textures/_ERROR_");
    textureStore()->createTextureFromFile(":model/textures/dev/devwhite");
}

void MainWindow::initMaterials()
{
    // For now the string must be identical.
    Renderer::RenderMaterialPointer material = materialStore()->createMaterial(":model/textures/dev/devwhite");
    m_iPlaceholderMaterial = material->materialStoreId();

    material->addTexture(Renderer::ShaderDefs::MainTexture, textureStore()->getTextureId(":model/textures/dev/devwhite"));
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

    QList<GenericBrush*> brushes = scene()->rootObject()->findChildren<GenericBrush*>();

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
    ModelLoaders::VTFLoader loader(materialStore(), textureStore());
    loadVpks();
    loader.loadMaterials(vpkFileCollection());
}
