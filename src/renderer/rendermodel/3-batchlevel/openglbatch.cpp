#include "openglbatch.h"
#include "shaders/shaderdefs.h"

namespace NS_RENDERER
{
    OpenGLBatch::OpenGLBatch(QOpenGLBuffer::UsagePattern usagePattern, const VertexFormat &format, int batchSize)
        : m_iUsagePattern(usagePattern),
          m_bCreated(false),
          m_VertexFormat(format),
          m_iBatchSize(batchSize),
          m_iObjectIdMask(maskFromNumberOfBits(bitsRequired(m_iBatchSize))),
          m_VertexBuffer(QOpenGLBuffer::VertexBuffer),
          m_IndexBuffer(QOpenGLBuffer::IndexBuffer),
          m_UniformBuffer(m_iUsagePattern),
          m_IndexPool(m_iBatchSize)
    {
        Q_ASSERT_X(m_iBatchSize >= 1, Q_FUNC_INFO, "Batch size must be at least 1!");
        setUsagePattern(m_iUsagePattern);
    }

    OpenGLBatch::~OpenGLBatch()
    {
        destroy();
    }

    void OpenGLBatch::create()
    {
        if ( m_bCreated )
            return;

        m_bCreated =
                m_VertexBuffer.create() &&
                m_IndexBuffer.create() &&
                m_UniformBuffer.create();
    }

    void OpenGLBatch::destroy()
    {
        if ( !m_bCreated )
            return;

        m_VertexBuffer.destroy();
        m_IndexBuffer.destroy();
        m_UniformBuffer.destroy();

        m_bCreated = false;
    }

    QOpenGLBuffer::UsagePattern OpenGLBatch::usagePattern() const
    {
        return m_iUsagePattern;
    }

    void OpenGLBatch::setUsagePattern(QOpenGLBuffer::UsagePattern usagePattern)
    {
        Q_ASSERT_X(!m_bCreated, Q_FUNC_INFO, "Cannot be set once batch is created!");

        m_iUsagePattern = usagePattern;
        m_VertexBuffer.setUsagePattern(m_iUsagePattern);
        m_IndexBuffer.setUsagePattern(m_iUsagePattern);
        m_UniformBuffer.setUsagePattern(m_iUsagePattern);
    }

    bool OpenGLBatch::isCreated() const
    {
        return m_bCreated;
    }

    const VertexFormat& OpenGLBatch::vertexFormat() const
    {
        return m_VertexFormat;
    }

    int OpenGLBatch::batchSize() const
    {
        return m_iBatchSize;
    }

    bool OpenGLBatch::isSingleItemBatch() const
    {
        return batchSize() == 1;
    }

    void OpenGLBatch::upload()
    {
        uploadVertices();
        uploadIndices();
        uploadUniforms();
    }

    void OpenGLBatch::uploadVertices()
    {
        m_VertexBuffer.bind();

        // TODO: Allocate and upload data

        m_VertexBuffer.release();
    }

    void OpenGLBatch::uploadIndices()
    {
        m_IndexBuffer.bind();

        // TODO: Allocate and upload data

        m_IndexBuffer.release();
    }

    void OpenGLBatch::uploadUniforms()
    {
        // Warning: The specific order of these commands may be important.
        // UBOs are finnicky.

        m_UniformBuffer.bind();
        m_UniformBuffer.allocate(/*TODO*/);
        m_UniformBuffer.release();

        m_UniformBuffer.bindToIndex(ShaderDefs::LocalUniformBlockBindingPoint);

        m_UniformBuffer.bind();

        // TODO: Upload data

        m_UniformBuffer.release();
    }
}
