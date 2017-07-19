#include "framebuffercopier.h"

#include "calliperutil/opengl/openglerrors.h"
#include "calliperutil/opengl/openglhelpers.h"
#include "calliperutil/array/arrayutil.h"

#include "rendersystem/global/rendersystem.h"

namespace
{
    const float vertexData[] =
    {
        -1,-1,
        1,-1,
        1,1,
        -1,1,
    };

    const quint32 indexData[] =
    {
        0,1,2,
        0,2,3,
    };

    const char* localVertexShaderSource =
            "#version 410 core\n"
            "layout (location=0) in vec2 vPosition;\n"
            "out vec2 fPosition;\n"
            "void main()\n"
            "{\n"
            "   gl_Position = vec4(vPosition.xy, 0.0, 1.0);\n"
            "   fPosition = (vPosition + vec2(1,1)) / 2.0;\n"
            "}\n"
            ;

    const char* localFragmentShaderSource =
            "#version 410 core\n"
            "in vec2 fPosition;\n"
            "layout(location = 0) out vec4 color;\n"
            "uniform sampler2D tex;\n"
            "void main() { color = texture(tex, fPosition); }\n"
            ;
}

FrameBufferCopier::FrameBufferCopier()
    : m_bCreatedSuccessfully(false),
      m_pShaderProgram(Q_NULLPTR),
      m_VAOID(0),
      m_pVertexBuffer(Q_NULLPTR),
      m_pIndexBuffer(Q_NULLPTR),
      m_nCreationContext(0)
{

}

FrameBufferCopier::~FrameBufferCopier()
{
    destroy();
}

bool FrameBufferCopier::create()
{
    if ( m_bCreatedSuccessfully )
    {
        return true;
    }

    Q_ASSERT_X(!RenderSystem::Global::renderSystemContextIsCurrent(), Q_FUNC_INFO, "Render system context should not be current!");

    GL_CURRENT_F;
    m_nCreationContext = reinterpret_cast<quint64>(f);

    GLTRY(f->glGenVertexArrays(1, &m_VAOID));
    GLTRY(f->glBindVertexArray(m_VAOID));

    m_pShaderProgram = new QOpenGLShaderProgram();
    if ( !m_pShaderProgram->create() )
    {
        Q_ASSERT(false);
        destroy();
        return false;
    }

    m_pShaderProgram->addShaderFromSourceCode(QOpenGLShader::Vertex, localVertexShaderSource);
    m_pShaderProgram->addShaderFromSourceCode(QOpenGLShader::Fragment, localFragmentShaderSource);
    m_pShaderProgram->link();
    m_pShaderProgram->bind();

    m_pVertexBuffer = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    if ( !m_pVertexBuffer->create() || !m_pVertexBuffer->bind() )
    {
        Q_ASSERT(false);
        destroy();
        return false;
    }

    m_pVertexBuffer->allocate(vertexData, 8 * sizeof(float));

    m_pIndexBuffer = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    if ( !m_pIndexBuffer->create() || !m_pIndexBuffer->bind() )
    {
        Q_ASSERT(false);
        destroy();
        return false;
    }

    m_pIndexBuffer->allocate(indexData, 6 * sizeof(quint32));

    m_pShaderProgram->enableAttributeArray(0);
    m_pShaderProgram->setAttributeBuffer("vPosition", GL_FLOAT, 0, 2);

    m_pIndexBuffer->release();
    m_pVertexBuffer->release();
    GLTRY(f->glBindVertexArray(0));


    m_bCreatedSuccessfully = true;
    return true;
}

void FrameBufferCopier::destroy()
{
    if ( !m_bCreatedSuccessfully )
    {
        return;
    }

    GL_CURRENT_F;
    const quint64 currentContext = reinterpret_cast<quint64>(f);
    Q_ASSERT(m_nCreationContext == 0 || currentContext == m_nCreationContext);

    if ( m_pVertexBuffer )
    {
        m_pVertexBuffer->release();
        delete m_pVertexBuffer;
        m_pVertexBuffer = Q_NULLPTR;
    }

    if ( m_pIndexBuffer )
    {
        m_pIndexBuffer->release();
        delete m_pIndexBuffer;
        m_pIndexBuffer = Q_NULLPTR;
    }

    if ( m_pShaderProgram )
    {
        m_pShaderProgram->release();
        delete m_pShaderProgram;
        m_pShaderProgram = Q_NULLPTR;
    }

    if ( m_VAOID != 0 )
    {
        GLTRY(f->glBindVertexArray(0));
        GLTRY(f->glDeleteVertexArrays(1, &m_VAOID));
    }

    m_bCreatedSuccessfully = false;
    m_nCreationContext = 0;
}

bool FrameBufferCopier::isCreated() const
{
    return m_bCreatedSuccessfully;
}

void FrameBufferCopier::draw(GLuint textureId)
{
    Q_ASSERT_X(!RenderSystem::Global::renderSystemContextIsCurrent(), Q_FUNC_INFO, "Render system context should not be current!");

    GL_CURRENT_F;
    Q_ASSERT(m_nCreationContext == 0 || reinterpret_cast<quint64>(f) == m_nCreationContext);

    if ( !m_bCreatedSuccessfully )
    {
        return;
    }

    if ( !m_pShaderProgram->bind() )
    {
        releaseAll();
        return;
    }

    GLTRY(f->glBindVertexArray(m_VAOID));

    if ( !m_pVertexBuffer->bind() || !m_pIndexBuffer->bind() )
    {
        releaseAll();
        return;
    }

    GLTRY(f->glBindTexture(GL_TEXTURE_2D, textureId));
    GLTRY(f->glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));

    releaseAll();
}

void FrameBufferCopier::releaseAll()
{
    GL_CURRENT_F;

    GLTRY(f->glBindTexture(GL_TEXTURE_2D, 0));
    m_pIndexBuffer->release();
    m_pVertexBuffer->release();
    m_pShaderProgram->release();

    GLTRY(f->glBindVertexArray(0));
}

quint64 FrameBufferCopier::creationContext() const
{
    return m_nCreationContext;
}
