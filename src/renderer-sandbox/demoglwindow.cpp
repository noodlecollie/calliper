#include "demoglwindow.h"
#include <QScreen>
#include <QOpenGLFunctions_4_1_Core>
#include "opengl/openglerrors.h"
#include <QtDebug>
#include "shaders/ishaderspec.h"
#include "shaders/shaderdefs.h"
#include "opengl/openglhelpers.h"
#include "shaders/debugscreenspaceshader.h"
#include "tempshader.h"
#include "geometry/geometrybuilder.h"
#include "irenderer.h"
#include "rendermodel/rendererinputobjectparams.h"

using namespace NS_RENDERER;

TempShader* debugShader = NULL;

class ShaderFunctor : public IShaderRetrievalFunctor
{
public:
    virtual OpenGLShaderProgram* operator ()(quint16) const override
    {
        return debugShader;
    }
};

ShaderFunctor* shaderFunctor = NULL;

OpenGLTexturePointer debugTexture;

class TextureFunctor : public ITextureRetrievalFunctor
{
public:
    virtual OpenGLTexturePointer operator ()(quint64) const override
    {
        // TODO: Fix me!
        return debugTexture;
    }
};

TextureFunctor* textureFunctor = NULL;

QVector<float> triangle(const QVector2D min, const QVector2D max)
{
    QVector<float> v;

    v.append(min.x());
    v.append(min.y());
    v.append(0);

    v.append(max.x());
    v.append(min.y());
    v.append(0);

    v.append((max.x() + min.x())/2.0f);
    v.append(max.y());
    v.append(0);

    return v;
}

QMatrix4x4 transMat(float x)
{
    return QMatrix4x4(1,0,0,x,
                      0,1,0,0,
                      0,0,1,0,
                      0,0,0,1);
}

void rendererUpdateObject(const QMatrix4x4 matrix, quint32 objectId, const GeometryBuilder &builder)
{
    Global::renderer()->updateObject(RendererInputObjectParams(
                               RenderModelKey(
                                   RenderModelPassKey(IRenderer::PASS_GENERAL),
                                   RenderModelBatchGroupKey(0, 0),
                                   MatrixBatchKey(matrix),
                                   MatrixBatchItemKey(objectId)
                                   ),
                               builder
                               ));
}

DemoGLWindow::DemoGLWindow()
{
    m_pTempSpec = NULL;
    m_pTexture = NULL;
}

DemoGLWindow::~DemoGLWindow()
{
    makeCurrent();
    Q_ASSERT_X(QOpenGLContext::currentContext(), Q_FUNC_INFO, "Need a current context to clean things up!");

    Global::shutdown();

    delete m_pTexture;
    m_pTexture = NULL;

    delete debugShader;
    debugShader = NULL;

    delete shaderFunctor;
    shaderFunctor = NULL;

    delete textureFunctor;
    textureFunctor = NULL;

    debugTexture.clear();

    GL_CURRENT_F;
    GLTRY(f->glDeleteBuffers(1, &m_iVAOID));

    doneCurrent();
}

void DemoGLWindow::initializeGL()
{
    qDebug() << OpenGLErrors::debugOpenGLCapabilities().toLatin1().constData();
    GL_CURRENT_F;

    GLTRY(f->glGenVertexArrays(1, &m_iVAOID));
    GLTRY(f->glBindVertexArray(m_iVAOID));

    debugShader = new TempShader();
    debugShader->construct();
    debugShader->bind();
    GLuint blockIndex = f->glGetUniformBlockIndex(debugShader->programId(), ShaderDefs::LOCAL_UNIFORM_BLOCK_NAME);
    f->glUniformBlockBinding(debugShader->programId(), blockIndex, ShaderDefs::LocalUniformBlockBindingPoint);
    debugShader->release();

    shaderFunctor = new ShaderFunctor();
    textureFunctor = new TextureFunctor();
    debugTexture = OpenGLTexturePointer(new OpenGLTexture(QImage(":/obsolete.png").mirrored()));

    Global::initialise();
    IRenderer* renderer = Global::renderer();
    renderer->setShaderFunctor(shaderFunctor);
    renderer->setTextureFunctor(textureFunctor);

    QVector<float> tri1 = triangle(QVector2D(-0.1f, -0.5f), QVector2D(0.1f, 0.5f));

    GLfloat cols[] = { 0,1,1,1, 1,0,1,1, 1,1,0,1 };
    GLfloat textureCoords[] = { 0,0, 1,0, 0.5f,1, };
    GLuint indices[] = { 0,1,2 };

    GeometryBuilder builder(0,0);
    GeometrySection& section = builder.currentSection();
    section.addPositions(tri1.constData(), tri1.count(), 3);
    section.add(GeometrySection::TextureCoordinateAttribute, textureCoords, 6);
    section.add(GeometrySection::ColorAttribute, cols, 12);
    section.addIndexTriangle(indices[0], indices[1], indices[2]);

    rendererUpdateObject(transMat(-0.7f), 0, builder);
    rendererUpdateObject(transMat(-0.6f), 1, builder);
    rendererUpdateObject(transMat(-0.5f), 2, builder);
    rendererUpdateObject(transMat(-0.4f), 3, builder);
    rendererUpdateObject(transMat(-0.3f), 4, builder);
    rendererUpdateObject(transMat(-0.2f), 5, builder);
    rendererUpdateObject(transMat(-0.1f), 6, builder);
    rendererUpdateObject(transMat(0.0f), 7, builder);

    /*
    m_pRenderModel = new RenderModelPass(shaderFunctor, textureFunctor);
    m_pRenderModel->create();
    m_pRenderModel->setUsagePattern(QOpenGLBuffer::DynamicDraw);
    m_pRenderModel->setAttributes(RenderModelPassAttributes(
                QMatrix4x4(2,0,0,0, 0,2,0,0, 0,0,2,0, 0,0,0,1),
                QMatrix4x4()
                ));
    m_pRenderModel->upload();

    m_pRenderModel->addItem(RenderModelBatchKey(0,0,transMat(-0.7f)), RenderModelBatchParams(builder.sections(), 0, transMat(-0.7f)));
    m_pRenderModel->addItem(RenderModelBatchKey(0,0,transMat(-0.6f)), RenderModelBatchParams(builder.sections(), 1, transMat(-0.6f)));
    m_pRenderModel->addItem(RenderModelBatchKey(0,0,transMat(-0.5f)), RenderModelBatchParams(builder.sections(), 2, transMat(-0.5f)));
    m_pRenderModel->addItem(RenderModelBatchKey(0,0,transMat(-0.4f)), RenderModelBatchParams(builder.sections(), 3, transMat(-0.4f)));
    m_pRenderModel->addItem(RenderModelBatchKey(0,0,transMat(-0.3f)), RenderModelBatchParams(builder.sections(), 4, transMat(-0.3f)));
    m_pRenderModel->addItem(RenderModelBatchKey(0,0,transMat(-0.2f)), RenderModelBatchParams(builder.sections(), 5, transMat(-0.2f)));
    m_pRenderModel->addItem(RenderModelBatchKey(0,0,transMat(-0.1f)), RenderModelBatchParams(builder.sections(), 6, transMat(-0.1f)));
    m_pRenderModel->addItem(RenderModelBatchKey(0,0,transMat(0.0f)), RenderModelBatchParams(builder.sections(), 7, transMat(0.0f)));
    m_pRenderModel->debugUploadAll();
    */
}

void DemoGLWindow::resizeGL(int w, int h)
{
    QOpenGLWindow::resizeGL(w,h);
}

void DemoGLWindow::paintGL()
{
    GL_CURRENT_F;

    const qreal retinaScale = devicePixelRatio();
    GLTRY(f->glViewport(0, 0, width() * retinaScale, height() * retinaScale));

    GLTRY(f->glClear(GL_COLOR_BUFFER_BIT));

    GLTRY(f->glBindVertexArray(m_iVAOID));

    /*
    m_pRenderModel->debugDraw();
    */

    GLTRY(f->glBindVertexArray(0));
}
