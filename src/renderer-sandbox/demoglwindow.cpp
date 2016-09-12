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

DemoGLWindow::DemoGLWindow()
{
    m_pTempSpec = NULL;
    m_pTexture = NULL;
}

DemoGLWindow::~DemoGLWindow()
{
    makeCurrent();
    Q_ASSERT_X(QOpenGLContext::currentContext(), Q_FUNC_INFO, "Need a current context to clean things up!");

    delete m_pTexture;
    m_pTexture = NULL;

    delete m_pRenderModel;
    m_pRenderModel = NULL;

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

    m_pRenderModel = new RenderModelPass(shaderFunctor, textureFunctor);

    QVector<float> tri1 = triangle(QVector2D(-0.1f, -0.5f), QVector2D(0.1f, 0.5f));

    GLfloat cols[] = { 1,0,0,1, 0,1,0,1, 0,0,1,1 };
    GLfloat textureCoords[] = { 0,0, 1,0, 0.5f,1, };
    GLuint indices[] = { 0,1,2 };

    GeometryBuilder builder;
    GeometrySection& section = builder.currentSection();
    section.addPositions(tri1.constData(), tri1.count(), 3);
    section.add(GeometrySection::TextureCoordinateAttribute, textureCoords, 6);
    section.add(GeometrySection::ColorAttribute, cols, 12);
    section.addIndexTriangle(indices[0], indices[1], indices[2]);

    RenderModelBatchKey key(0, 0);
//    m_pRenderModel->addItem(key, RenderModelBatchParams(3, tri1.constData(), 3, indices, transMat(-0.7f), NULL, cols, textureCoords));
//    m_pRenderModel->addItem(key, RenderModelBatchParams(3, tri1.constData(), 3, indices, transMat(-0.6f), NULL, cols, textureCoords));
//    m_pRenderModel->addItem(key, RenderModelBatchParams(3, tri1.constData(), 3, indices, transMat(-0.5f), NULL, cols, textureCoords));
//    m_pRenderModel->addItem(key, RenderModelBatchParams(3, tri1.constData(), 3, indices, transMat(-0.4f), NULL, cols, textureCoords));
//    m_pRenderModel->addItem(key, RenderModelBatchParams(3, tri1.constData(), 3, indices, transMat(-0.3f), NULL, cols, textureCoords));
//    m_pRenderModel->addItem(key, RenderModelBatchParams(3, tri1.constData(), 3, indices, transMat(-0.2f), NULL, cols, textureCoords));
//    m_pRenderModel->addItem(key, RenderModelBatchParams(3, tri1.constData(), 3, indices, transMat(-0.1f), NULL, cols, textureCoords));
//    m_pRenderModel->addItem(key, RenderModelBatchParams(3, tri1.constData(), 3, indices, transMat(0.0f), NULL, cols, textureCoords));
    m_pRenderModel->addItem(key, RenderModelBatchParams(builder.sections(), QMatrix4x4()));
    QVector<float> vertexData;
    QVector<quint32> indexData;
    QVector<float> uniformData;
    m_pRenderModel->debugUploadAll(/*vertexData, uniformData, indexData*/);
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

    m_pRenderModel->debugDraw();

    GLTRY(f->glBindVertexArray(0));
}
