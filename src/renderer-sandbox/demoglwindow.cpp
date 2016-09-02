#include "demoglwindow.h"
#include <QScreen>
#include <QOpenGLFunctions_4_1_Core>
#include "openglerrors.h"
#include <QtDebug>
#include "rendermodelbatch.h"
#include "ishaderspec.h"
#include "shaderdefs.h"
#include "openglhelpers.h"

using namespace NS_RENDERER;

class TempSpec : public IShaderSpec
{
public:
    virtual ~TempSpec() {}

    virtual int positionComponents() const { return 3; }
    virtual int normalComponents() const { return 0; }
    virtual int colorComponents() const { return 4; }
    virtual int textureCoordinateComponents() const { return 2; }
    virtual int maxBatchedItems() const { return 8; }
};

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

static const char *vertexShaderSource =
        "#version 410 core\n"
        "layout (location=0) in vec4 vPosition;\n"
        "layout (location=2) in vec4 vColour;\n"
        "layout (location=3) in vec2 vTexCoord;\n"
        "layout (std140) uniform LocalUniformBlock\n"
        "{\n"
        "   mat4 modelToWorldMatrices[8];\n"
        "};\n"
        "out vec4 fColour;\n"
        "out vec2 fTexCoord;\n"
        "void main()\n"
        "{\n"
        "   uint id = uint(vPosition.z);\n"
        "   gl_Position = modelToWorldMatrices[id] * vec4(vPosition.xy, 0, 1);\n"
        "   fColour = vColour;\n"
        "   fTexCoord = vTexCoord;\n"
        "}\n"
    ;

static const char *fragmentShaderSource =
        "#version 410 core\n"
        "in vec4 fColour;\n"
        "in vec2 fTexCoord;\n"
        "layout(location = 0) out vec4 color;\n"
        "uniform sampler2D tex;\n"
        "void main()\n"
        "{\n"
        "   color = texture(tex, fTexCoord) /* * fColour*/;\n"
        "}\n"
    ;

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

    delete m_pBatch;
    m_pBatch = NULL;

    delete m_pTempSpec;
    m_pTempSpec = NULL;

    delete m_program;
    m_program = NULL;

    GL_CURRENT_F;
    GLTRY(f->glDeleteBuffers(1, &m_iVAOID));

    doneCurrent();
}

void DemoGLWindow::initializeGL()
{
    qDebug() << OpenGLErrors::debugOpenGLCapabilities().toLatin1().constData();
    GL_CURRENT_F;

    m_TextureImage = QImage(qApp->applicationDirPath() + "/../../../src/application/resource/textures/dev/obsolete.png");
    qDebug().nospace() << "Texture image: " << m_TextureImage.width() << "x" << m_TextureImage.height();

    m_pTexture = new QOpenGLTexture(m_TextureImage.mirrored());

    GLTRY(f->glGenVertexArrays(1, &m_iVAOID));
    GLTRY(f->glBindVertexArray(m_iVAOID));

    GLTRY(m_program = new QOpenGLShaderProgram(QOpenGLContext::currentContext()));
    GLTRY(m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource));
    GLTRY(m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource));
    GLTRY(m_program->link());
    GLTRY(m_program->bind());

    m_pTempSpec = new TempSpec();

    GLTRY(m_pBatch = new RenderModelBatch(QOpenGLBuffer::DynamicDraw, m_pTempSpec, m_program, this));
    GLTRY(m_pBatch->create());

    QVector<float> tri1 = triangle(QVector2D(-0.1f, -0.5f), QVector2D(0.1f, 0.5f));

    GLfloat cols[] = { 1,0,0,1, 0,1,0,1, 0,0,1,1 };
    GLfloat textureCoords[] = { 0,0, 1,0, 0.5f,1, };
    GLuint indices[] = { 0,1,2 };

    GLTRY(m_pBatch->addItem(RenderModelBatchParams(3, tri1.constData(), 3, indices, transMat(-0.7f), NULL, cols, textureCoords)));
    GLTRY(m_pBatch->addItem(RenderModelBatchParams(3, tri1.constData(), 3, indices, transMat(-0.6f), NULL, cols, textureCoords)));
    GLTRY(m_pBatch->addItem(RenderModelBatchParams(3, tri1.constData(), 3, indices, transMat(-0.5f), NULL, cols, textureCoords)));
    GLTRY(m_pBatch->addItem(RenderModelBatchParams(3, tri1.constData(), 3, indices, transMat(-0.4f), NULL, cols, textureCoords)));
    GLTRY(m_pBatch->addItem(RenderModelBatchParams(3, tri1.constData(), 3, indices, transMat(-0.3f), NULL, cols, textureCoords)));
    GLTRY(m_pBatch->addItem(RenderModelBatchParams(3, tri1.constData(), 3, indices, transMat(-0.2f), NULL, cols, textureCoords)));
    GLTRY(m_pBatch->addItem(RenderModelBatchParams(3, tri1.constData(), 3, indices, transMat(-0.1f), NULL, cols, textureCoords)));
    GLTRY(m_pBatch->addItem(RenderModelBatchParams(3, tri1.constData(), 3, indices, transMat(0.0f), NULL, cols, textureCoords)));

    GLTRY(m_pBatch->upload());
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

    GLTRY(m_program->enableAttributeArray(ShaderDefs::PositionAttribute));
    GLTRY(m_program->enableAttributeArray(ShaderDefs::ColorAttribute));
    GLTRY(m_program->enableAttributeArray(ShaderDefs::TextureCoordinateAttribute));

    GLTRY(m_pBatch->bindDraw());
    GLTRY(m_pTexture->bind(0));
    GLTRY(m_pBatch->setAttributePointers());
    GLTRY(m_pBatch->draw());
    GLTRY(m_pTexture->release());
    GLTRY(m_pBatch->releaseDraw());

    GLTRY(m_program->disableAttributeArray(ShaderDefs::PositionAttribute));
    GLTRY(m_program->disableAttributeArray(ShaderDefs::ColorAttribute));
    GLTRY(m_program->disableAttributeArray(ShaderDefs::TextureCoordinateAttribute));

    GLTRY(f->glBindVertexArray(0));
}
