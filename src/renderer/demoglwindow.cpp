#include "demoglwindow.h"
#include <QScreen>
#include <QOpenGLFunctions_4_1_Core>
#include "openglerrors.h"
#include <QtDebug>
#include "rendermodelbatch.h"
#include "ishaderspec.h"

namespace NS_RENDERER
{
    class TempSpec : public IShaderSpec
    {
    public:
        virtual ~TempSpec() {}

        virtual int positionComponents() const { return 2; }
        virtual int normalComponents() const { return 3; }
        virtual int colorComponents() const { return 4; }
        virtual int textureCoordinateComponents() const { return 2; }
    };

    static const char *vertexShaderSource =
            "#version 410 core\n"
            "in vec2 vPosition;"
            "in vec4 vColour;"
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
        QOpenGLFunctions_4_1_Core* f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_1_Core>();

        GLTRY(f->glGenVertexArrays(1, &m_vao));
        GLTRY(f->glBindVertexArray(m_vao));

        GLTRY(m_program = new QOpenGLShaderProgram(QOpenGLContext::currentContext()));
        GLTRY(m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource));
        GLTRY(m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource));
        GLTRY(m_program->link());
        GLTRY(m_program->bind());
        GLTRY(m_posAttr = m_program->attributeLocation("vPosition"));
        GLTRY(m_colAttr = m_program->attributeLocation("vColour"));

#if 0
        GLfloat atts[] = {
            -1,-1,      // 0 1
            1,-1,       // 2 3
            0,1,        // 4 5
        };

        m_pVertexBuffer = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
        m_pVertexBuffer->create();
        m_pVertexBuffer->bind();
        m_pVertexBuffer->allocate(atts, 6*sizeof(GLfloat));

        GLfloat cols[] = {
            1,0,0,1,
            0,1,0,1,
            0,0,1,1,
        };

        m_pColourBuffer = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
        m_pColourBuffer->create();
        m_pColourBuffer->bind();
        m_pColourBuffer->allocate(cols, 12*sizeof(GLfloat));

        GLushort indices[] =
        {
            0,1,2
        };

        m_pIndexBuffer = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
        m_pIndexBuffer->create();
        m_pIndexBuffer->bind();
        m_pIndexBuffer->allocate(indices, 3*sizeof(GLushort));
#endif

        GLTRY(m_pBatch = new RenderModelBatch(QOpenGLBuffer::DynamicDraw, this));
        m_pTempSpec = new TempSpec();
        m_pBatch->setShaderSpec(m_pTempSpec);
        GLTRY(m_pBatch->create());

        GLfloat positions[] = { -1,-1, 1,-1, 0,1, };
        GLfloat cols[] = { 1,0,0,1, 0,1,0,1, 0,0,1,1 };
        GLuint indices[] = { 0,1,2 };
        GLTRY(m_pBatch->addItem(RenderModelBatchParams(3, positions, 3, indices, QMatrix4x4(), NULL, cols, NULL)));
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
#if 0
        m_program->enableAttributeArray(m_posAttr);
        m_pVertexBuffer->bind();
        m_program->setAttributeBuffer(m_posAttr, GL_FLOAT, 0*sizeof(GLfloat), 2);

        m_program->enableAttributeArray(m_colAttr);
        m_pColourBuffer->bind();
        m_program->setAttributeBuffer(m_colAttr, GL_FLOAT, 0*sizeof(GLfloat), 4);

        GLTRY(f->glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, (void*)0));

        m_program->disableAttributeArray(m_posAttr);
        m_program->disableAttributeArray(m_colAttr);
#endif

        QOpenGLBuffer v = m_pBatch->vertexBuffer();
        QOpenGLBuffer i = m_pBatch->indexBuffer();

        GLTRY(m_program->enableAttributeArray(m_posAttr));
        GLTRY(m_program->enableAttributeArray(m_colAttr));
        GLTRY(v.bind());
        GLTRY(m_program->setAttributeBuffer(m_posAttr, GL_FLOAT, 0*sizeof(GLfloat), m_pTempSpec->positionComponents()));
        GLTRY(m_program->setAttributeBuffer(m_colAttr, GL_FLOAT,
                                            (m_pBatch->localPositionCount() + m_pBatch->localNormalCount())*sizeof(GLfloat),
                                            m_pTempSpec->colorComponents()));

        GLTRY(i.bind());

        GLTRY(f->glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)0));

        GLTRY(m_program->disableAttributeArray(m_posAttr));
        GLTRY(m_program->disableAttributeArray(m_colAttr));
    }
}
