#include "customopenglwidget.h"

#include <QOpenGLPaintDevice>
#include <QGuiApplication>

#include "calliperutil/opengl/openglerrors.h"
#include "calliperutil/opengl/openglhelpers.h"

namespace
{
    const char* vertexShader =
            "#version 410 core\n"
            "layout (location=0) in vec2 vPosition;\n"
            "void main()\n"
            "{\n"
            "   gl_Position = vec4(vPosition.xy, 0.0, 1.0);\n"
            "}\n"
            ;

    const char* fragmentShader =
            "#version 410 core\n"
            "out vec4 color;"
            "void main()\n"
            "{\n"
            "   color = vec4(1,0,0,1);\n"
            "}\n"
            ;

    const float vertices[] =
    {
        -1,-1,
        1,-1,
        0,1,
    };

    const quint32 indices[] =
    {
        0,1,2,
    };

    template<typename T>
    inline void deleteResource(T& pointer)
    {
        delete pointer;
        pointer = Q_NULLPTR;
    }

    class CustomOpenGLWidgetPaintDevice : public QOpenGLPaintDevice
    {
    public:
        CustomOpenGLWidgetPaintDevice(CustomOpenGLWidget *widget)
            : QOpenGLPaintDevice(widget->size() * (int)qApp->devicePixelRatio()),
              m_pWidget(widget)
        {
        }

        void ensureActiveTarget() override
        {
            if ( !m_pWidget->isInitialised() )
            {
                return;
            }

            if (QOpenGLContext::currentContext() != m_pWidget->context())
            {
                m_pWidget->makeCurrentWithSurface();
            }

            m_pWidget->bindFBO();
            m_pWidget->flagFlushPending();
        }

    private:
        CustomOpenGLWidget* m_pWidget;
    };
}

CustomOpenGLWidget::CustomOpenGLWidget(QOpenGLContext* context, QWidget *parent, Qt::WindowFlags f)
    : QWidget(parent, f),
      m_pShaderProgram(Q_NULLPTR),
      m_pVertexBuffer(Q_NULLPTR),
      m_pIndexBuffer(Q_NULLPTR),
      m_nVAOID(0),
      m_bInitialised(false),
      m_pContext(context),
      m_bFlushPending(false),
      m_pFrameBuffer(Q_NULLPTR),
      m_pOffscreenSurface(Q_NULLPTR)
{
    Q_ASSERT(m_pContext);

    QList<QScreen*> screens = qApp->screens();
    Q_ASSERT(!screens.isEmpty());

    m_pOffscreenSurface = new QOffscreenSurface(screens.at(0));
    m_pOffscreenSurface->setFormat(QSurfaceFormat::defaultFormat());
    m_pOffscreenSurface->create();

    makeCurrentWithSurface();
    recreateFrameBuffer();
    initialiseTriangleResources();
    doneCurrent();

    m_bInitialised = true;
}

CustomOpenGLWidget::~CustomOpenGLWidget()
{
    m_bInitialised = false;

    makeCurrentWithSurface();
    destroyTriangleResources();
    deleteResource(m_pFrameBuffer);
    doneCurrent();

    deleteResource(m_pOffscreenSurface);
}

bool CustomOpenGLWidget::isInitialised() const
{
    return m_bInitialised;
}

QOpenGLContext* CustomOpenGLWidget::context() const
{
    return m_pContext;
}

void CustomOpenGLWidget::makeCurrentWithSurface()
{
    m_pContext->makeCurrent(m_pOffscreenSurface);
}

void CustomOpenGLWidget::doneCurrent()
{
    m_pContext->doneCurrent();
}

void CustomOpenGLWidget::initialiseTriangleResources()
{
    GL_CURRENT_F;
    GLTRY(f->glGenVertexArrays(1, &m_nVAOID));
    GLTRY(f->glBindVertexArray(m_nVAOID));

    m_pShaderProgram = new QOpenGLShaderProgram();
    m_pShaderProgram->create();
    m_pShaderProgram->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShader);
    m_pShaderProgram->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShader);
    m_pShaderProgram->link();
    m_pShaderProgram->bind();

    m_pVertexBuffer = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    m_pVertexBuffer->create();
    m_pVertexBuffer->bind();
    m_pVertexBuffer->allocate(vertices, 6 * sizeof(float));
    m_pVertexBuffer->release();

    m_pIndexBuffer = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    m_pIndexBuffer->create();
    m_pIndexBuffer->bind();
    m_pIndexBuffer->allocate(indices, 3 * sizeof(quint32));
    m_pIndexBuffer->release();

    GLTRY(f->glBindVertexArray(0));
}

void CustomOpenGLWidget::destroyTriangleResources()
{
    deleteResource(m_pIndexBuffer);
    deleteResource(m_pVertexBuffer);
    deleteResource(m_pShaderProgram);

    GL_CURRENT_F;
    GLTRY(f->glDeleteVertexArrays(1, &m_nVAOID));
    m_nVAOID = 0;
}

void CustomOpenGLWidget::drawTriangle()
{
    GL_CURRENT_F;
    GLTRY(f->glBindVertexArray(m_nVAOID));

    m_pShaderProgram->bind();
    m_pVertexBuffer->bind();
    m_pIndexBuffer->bind();

    m_pShaderProgram->enableAttributeArray(0);
    m_pShaderProgram->setAttributeBuffer(0, GL_FLOAT, 0, 2);

    GLTRY(f->glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0));

    m_pShaderProgram->disableAttributeArray(0);
    m_pIndexBuffer->release();
    m_pVertexBuffer->release();
    m_pShaderProgram->release();

    GLTRY(f->glBindVertexArray(0));
}

void CustomOpenGLWidget::recreateFrameBuffer()
{
    deleteResource(m_pFrameBuffer);

    QSize fboSize = size() * (int)qApp->devicePixelRatio();

    QOpenGLFramebufferObjectFormat format;
    format.setAttachment(QOpenGLFramebufferObject::Depth);
    format.setSamples(0);

    m_pFrameBuffer = new QOpenGLFramebufferObject(fboSize, format);
}

void CustomOpenGLWidget::bindFBO()
{
    if ( !m_pFrameBuffer )
    {
        return;
    }

    m_pFrameBuffer->bind();
}

void CustomOpenGLWidget::releaseFBO()
{
    if ( !m_pFrameBuffer )
    {
        return;
    }

    m_pFrameBuffer->release();
}

void CustomOpenGLWidget::flagFlushPending()
{
    m_bFlushPending = true;
}

void CustomOpenGLWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    makeCurrentWithSurface();

    QOpenGLPaintDevice paintDevice(m_pFrameBuffer->size());
    QPainter painter(&paintDevice);
    painter.beginNativePainting();

    drawTriangle();

    painter.endNativePainting();
    doneCurrent();
}
