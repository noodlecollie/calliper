#include "demoglwindow.h"
#include <QScreen>
#include <QOpenGLFunctions_4_1_Core>
#include "openglerrors.h"
#include <QtDebug>
#include "rendermodelbatch.h"
#include "ishaderspec.h"
#include "shaderdefs.h"

namespace NS_RENDERER
{
    class TempSpec : public IShaderSpec
    {
    public:
        virtual ~TempSpec() {}

        virtual int positionComponents() const { return 2; }
        virtual int normalComponents() const { return 0; }
        virtual int colorComponents() const { return 4; }
        virtual int textureCoordinateComponents() const { return 0; }
        virtual int maxBatchedItems() const { return 8; }
    };

    static const char *vertexShaderSource =
            "#version 410 core\n"
            "layout (location=0) in vec2 vPosition;"
            "layout (location=2) in vec4 vColour;"
            "out vec4 fColour;"
            "void main() { gl_Position = vec4(vPosition, 0, 1); fColour = vColour; }"
        ;

    static const char *fragmentShaderSource =
            "#version 410 core\n"
            "in vec4 fColour;"
            "layout(location = 0) out vec4 color;"
            "void main() { color = fColour; }"
        ;

    DemoGLWindow::DemoGLWindow()
    {
        m_pTempSpec = NULL;
    }

    DemoGLWindow::~DemoGLWindow()
    {
        delete m_pTempSpec;
        m_pTempSpec = NULL;
    }

    void DemoGLWindow::initializeGL()
    {
        qDebug() << OpenGLErrors::debugOpenGLCapabilities().toLatin1().constData();

        GLTRY(m_program = new QOpenGLShaderProgram(QOpenGLContext::currentContext()));
        GLTRY(m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource));
        GLTRY(m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource));
        GLTRY(m_program->link());
        GLTRY(m_program->bind());

        m_pTempSpec = new TempSpec();

        GLTRY(m_pBatch = new RenderModelBatch(QOpenGLBuffer::DynamicDraw, m_pTempSpec, m_program, this));
        GLTRY(m_pBatch->create());

        GLfloat positions1[] = { -1,-1, 0,-1, -0.5f,1, };
        GLfloat positions2[] = { 0,-1, 1,-1, 0.5f,1 };
        GLfloat cols[] = { 1,0,0,1, 0,1,0,1, 0,0,1,1 };
        GLuint indices[] = { 0,1,2 };
        GLTRY(m_pBatch->addItem(RenderModelBatchParams(3, positions1, 3, indices, QMatrix4x4(), NULL, cols, NULL)));
        GLTRY(m_pBatch->addItem(RenderModelBatchParams(3, positions2, 3, indices, QMatrix4x4(), NULL, cols, NULL)));
        GLTRY(m_pBatch->upload());
    }

    void DemoGLWindow::resizeGL(int w, int h)
    {
        QOpenGLWindow::resizeGL(w,h);
    }

    void DemoGLWindow::paintGL()
    {
        QOpenGLFunctions_4_1_Core* f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_1_Core>();

        const qreal retinaScale = devicePixelRatio();
        GLTRY(f->glViewport(0, 0, width() * retinaScale, height() * retinaScale));

        GLTRY(f->glClear(GL_COLOR_BUFFER_BIT));

        // These should be set when beginning a phase with a shader.
        GLTRY(m_program->enableAttributeArray(ShaderDefs::PositionAttribute));
        GLTRY(m_program->enableAttributeArray(ShaderDefs::ColorAttribute));

        GLTRY(m_pBatch->setAttributePointers());
        GLTRY(m_pBatch->draw());

        // These should be disabled once the phase with one shader has finished.
        GLTRY(m_program->disableAttributeArray(ShaderDefs::PositionAttribute));
        GLTRY(m_program->disableAttributeArray(ShaderDefs::ColorAttribute));
    }
}
