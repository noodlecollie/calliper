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

void buildObjects()
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

    QMatrix4x4 scale = scaleMat(QVector2D(1.0f/5.0f, 1.0f/5.0f));

    quint32 id = 0;
    for ( int i = 0; i < 10; i++ )
    {
        for ( int j = 0; j < 10; j++ )
        {
            float xTrans = -1.0f + ((1.8f * (float)i)/9.0f);
            float yTrans = -1.0f + ((1.8f * (float)j)/9.0f);
            QMatrix4x4 trans = transMat(QVector2D(xTrans, yTrans));
            section.setModelToWorldMatrix(trans * scale);
//            if ( i == 2 && j == 2 )
//            {
//                GeometryBuilder builder2(1,1, QMatrix4x4());
//                GeometrySection& section2 = builder.currentSection();
//                section2.addPositions(tri1.constData(), tri1.count(), 3);
//                section2.add(GeometrySection::TextureCoordinateAttribute, textureCoords, 6);
//                section2.add(GeometrySection::ColorAttribute, cols2, 12);
//                section2.addIndexTriangle(indices[0], indices[1], indices[2]);
//                renderer->updateObject(RendererInputObjectParams(id++, IRenderer::PASS_GENERAL, builder2));
//            }
//            else
//            {
                renderer->updateObject(RendererInputObjectParams(id++, IRenderer::PASS_GENERAL, builder));
//            }
        }
    }
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

    GLTRY(buildObjects());
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

    GLTRY(Global::renderer()->draw(RendererDrawParams()));

    GLTRY(f->glBindVertexArray(0));
}
