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

QMatrix4x4 transMat(const QVector2D &trans)
{
    return QMatrix4x4(1,0,0,trans.x(),
                      0,1,0,trans.y(),
                      0,0,1,0,
                      0,0,0,1);
}

QMatrix4x4 scaleMat(const QVector2D &scale)
{
    return QMatrix4x4(scale.x(),0,0,0,
                      0,scale.y(),0,0,
                      0,0,1,0,
                      0,0,0,1);
}

void buildObjects(int dim)
{
    IRenderer* renderer = Global::renderer();

    QVector<float> tri1 = triangle(QVector2D(0, 0), QVector2D(1, 1));

    GLfloat cols[] = { 0,1,1,1, 1,0,1,1, 1,1,0,1 };
    GLfloat cols2[] = { 1,0,0,1, 1,0,0,1, 1,0,0,1 };
    GLfloat textureCoords[] = { 0,0, 1,0, 0.5f,1, };
    GLuint indices[] = { 0,1,2 };

    GeometryBuilder builder(1,1, QMatrix4x4());
    GeometrySection& section = builder.currentSection();
    section.addPositions(tri1.constData(), tri1.count(), 3);
    section.add(GeometrySection::TextureCoordinateAttribute, textureCoords, 6);
    section.add(GeometrySection::ColorAttribute, cols, 12);
    section.addIndexTriangle(indices[0], indices[1], indices[2]);

    QMatrix4x4 scale = scaleMat(QVector2D(2.0f/(float)dim, 2.0f/(float)dim));

    quint32 id = 0;
    for ( int i = 0; i < dim; i++ )
    {
        for ( int j = 0; j < dim; j++ )
        {
            float xTrans = -1.0f + ((((1.0f - (2.0f/(float)dim)) + 1.0f) * (float)i)/(float)(dim-1));
            float yTrans = -1.0f + ((((1.0f - (2.0f/(float)dim)) + 1.0f) * (float)j)/(float)(dim-1));
            QMatrix4x4 trans = transMat(QVector2D(xTrans, yTrans));
            section.setModelToWorldMatrix(trans * scale);
            renderer->updateObject(RendererInputObjectParams(id++, IRenderer::PASS_GENERAL, builder));
        }
    }
}

DemoGLWindow::DemoGLWindow()
{
    m_pTempSpec = NULL;
    m_pTexture = NULL;

    m_Timer.setInterval((int)(10.0f/6.0f));
    m_Timer.setSingleShot(false);
    connect(&m_Timer, SIGNAL(timeout()), this, SLOT(update()));
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

    debugShader = new TempShader(1);
    debugShader->construct();
    debugShader->bind();
    GLuint blockIndex = f->glGetUniformBlockIndex(debugShader->programId(), ShaderDefs::LOCAL_UNIFORM_BLOCK_NAME);
    f->glUniformBlockBinding(debugShader->programId(), blockIndex, ShaderDefs::LocalUniformBlockBindingPoint);
    debugShader->release();

    shaderFunctor = new ShaderFunctor();
    textureFunctor = new TextureFunctor();
    debugTexture = OpenGLTexturePointer(new OpenGLTexture(1, QImage(":/obsolete.png").mirrored()));

    Global::initialise();
    IRenderer* renderer = Global::renderer();
    renderer->setShaderFunctor(shaderFunctor);
    renderer->setTextureFunctor(textureFunctor);

    m_iTris = 120;
    GLTRY(buildObjects(m_iTris));

    m_FrameTime = QTime::currentTime();
    m_Timer.start();
}

void DemoGLWindow::resizeGL(int w, int h)
{
    QOpenGLWindow::resizeGL(w,h);
}

void DemoGLWindow::paintGL()
{
    QTime curTime = QTime::currentTime();
    int oldMs = m_FrameTime.msecsSinceStartOfDay();
    int newMs = curTime.msecsSinceStartOfDay();
    qDebug() << "FPS:" << (1000.0f/(float)(newMs - oldMs)) << "with" << (m_iTris * m_iTris) << "triangles";
    m_FrameTime = curTime;

    GL_CURRENT_F;

    const qreal retinaScale = devicePixelRatio();
    GLTRY(f->glViewport(0, 0, width() * retinaScale, height() * retinaScale));

    GLTRY(f->glClear(GL_COLOR_BUFFER_BIT));

    GLTRY(f->glBindVertexArray(m_iVAOID));

    GLTRY(Global::renderer()->draw(RendererDrawParams()));

    GLTRY(f->glBindVertexArray(0));
}
