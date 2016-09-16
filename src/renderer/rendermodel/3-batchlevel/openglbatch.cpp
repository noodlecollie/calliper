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
          m_UniformBuffer(m_iUsagePattern)
    {
        Q_ASSERT_X(m_iBatchSize >= 1, Q_FUNC_INFO, "Batch size must be at least 1!");
        setUsagePattern(m_iUsagePattern);
    }

    OpenGLBatch::~OpenGLBatch()
    {
        destroy();
        clearMatrixBatches();
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

    int OpenGLBatch::matrixBatchCount() const
    {
        return m_MatrixBatches.count();
    }

    bool OpenGLBatch::matrixBatchLimitReached() const
    {
        return matrixBatchCount() >= m_iBatchSize;
    }

    MatrixBatch* OpenGLBatch::createMatrixBatch()
    {
        Q_ASSERT_X(!matrixBatchLimitReached(), Q_FUNC_INFO, "No more space to add matrix batches!");

        if ( matrixBatchLimitReached() )
            return NULL;

        m_MatrixBatches.append(new MatrixBatch());
        return m_MatrixBatches.back();
    }

    void OpenGLBatch::destroyMatrixBatch(int index)
    {
        if ( index < 0 || index >= m_MatrixBatches.count() )
            return;

        delete m_MatrixBatches.takeAt(index);
    }

    MatrixBatch* OpenGLBatch::matrixBatchAt(int index) const
    {
        return m_MatrixBatches.at(index);
    }

    void OpenGLBatch::clearMatrixBatches()
    {
        qDeleteAll(m_MatrixBatches);
        m_MatrixBatches.clear();
    }
}
