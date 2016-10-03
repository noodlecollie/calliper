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
#include "scene/scene.h"
#include "sceneobjects/debugcube.h"

using namespace NS_RENDERER;

TempShader* debugShader = nullptr;

enum Passes
{
    PASS_GENERAL = 0
};

class ShaderFunctor : public IShaderRetrievalFunctor
{
public:
    virtual OpenGLShaderProgram* operator ()(quint16) const override
    {
        return debugShader;
    }
};

ShaderFunctor* shaderFunctor = nullptr;

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

TextureFunctor* textureFunctor = nullptr;

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

    quint32 id = 1;
    for ( int i = 0; i < dim; i++ )
    {
        for ( int j = 0; j < dim; j++ )
        {
            float xTrans = -1.0f + ((((1.0f - (2.0f/(float)dim)) + 1.0f) * (float)i)/(float)(dim-1));
            float yTrans = -1.0f + ((((1.0f - (2.0f/(float)dim)) + 1.0f) * (float)j)/(float)(dim-1));
            QMatrix4x4 trans = transMat(QVector2D(xTrans, yTrans));
            builder.setModelToWorldMatrix(trans * scale);
            renderer->updateObject(RendererInputObjectParams(id, PASS_GENERAL, builder));
            id++;
        }
    }
}

DemoGLWindow::DemoGLWindow()
{
    m_pTempSpec = nullptr;
    m_pTexture = nullptr;
    m_iCounter = 1;
    m_iTris = 1;

    m_Timer.setInterval((int)(10.0f/6.0f));
    m_Timer.setSingleShot(false);
    connect(&m_Timer, SIGNAL(timeout()), this, SLOT(update()));

    m_HidingTimer.setInterval(50);
    m_HidingTimer.setSingleShot(false);
    connect(&m_HidingTimer, &QTimer::timeout, this, &DemoGLWindow::timeout);
}

DemoGLWindow::~DemoGLWindow()
{
    makeCurrent();
    Q_ASSERT_X(QOpenGLContext::currentContext(), Q_FUNC_INFO, "Need a current context to clean things up!");

    Global::shutdown();

    delete m_pTexture;
    m_pTexture = nullptr;

    delete debugShader;
    debugShader = nullptr;

    delete shaderFunctor;
    shaderFunctor = nullptr;

    delete textureFunctor;
    textureFunctor = nullptr;

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

    m_iTris = 15;
//    GLTRY(buildObjects(m_iTris));

    {
        using namespace NS_MODEL;

        m_pScene = new Scene(this);
        m_pSceneObject = m_pScene->createSceneObject<DebugCube>(nullptr);
        m_pSceneObject->setRadius(0.2f);
        m_pSceneObject->hierarchy().setPosition(QVector3D(0, 0, -0.3f));

        GeometryBuilder builder(1,1, m_pSceneObject->hierarchy().parentToLocal());
        m_pSceneObject->rendererUpdate(builder);
        renderer->updateObject(RendererInputObjectParams(1, PASS_GENERAL, builder));
    }

    m_FrameTime = QTime::currentTime();
    m_Timer.start();
    m_HidingTimer.start();
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

void DemoGLWindow::timeout()
{
    IRenderer* renderer = Global::renderer();
    makeCurrent();

    renderer->clearObjectFlags(m_iCounter, HiddenObjectFlag);

    ++m_iCounter;
    if ( m_iCounter > (m_iTris*m_iTris) )
    {
        m_iCounter = 1;
    }

    renderer->setObjectFlags(m_iCounter, HiddenObjectFlag);
    doneCurrent();
}
