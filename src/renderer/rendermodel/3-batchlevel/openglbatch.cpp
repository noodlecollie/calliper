#include "openglbatch.h"
#include "shaders/shaderdefs.h"

namespace NS_RENDERER
{
    OpenGLBatch::OpenGLBatch(QOpenGLBuffer::UsagePattern usagePattern, const IShaderSpec* shaderSpec)
        : m_iUsagePattern(usagePattern),
          m_bCreated(false),
          m_pShaderSpec(shaderSpec),
          m_iObjectIdMask(maskFromNumberOfBits(bitsRequired(m_pShaderSpec->maxBatchedItems()))),
          m_VertexBuffer(QOpenGLBuffer::VertexBuffer),
          m_IndexBuffer(QOpenGLBuffer::IndexBuffer),
          m_UniformBuffer(m_iUsagePattern)
    {
        Q_ASSERT(shaderSpec);

        m_VertexBuffer.setUsagePattern(m_iUsagePattern);
        m_IndexBuffer.setUsagePattern(m_iUsagePattern);
        m_UniformBuffer.setUsagePattern(m_iUsagePattern);
    }

    OpenGLBatch::~OpenGLBatch()
    {
        clearMatrixBatches();
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

    bool OpenGLBatch::isCreated() const
    {
        return m_bCreated;
    }

    const IShaderSpec* OpenGLBatch::shaderSpec() const
    {
        return m_pShaderSpec;
    }

    VertexFormat OpenGLBatch::vertexFormat() const
    {
        return m_pShaderSpec->vertexFormat();
    }

    int OpenGLBatch::batchSize() const
    {
        return m_pShaderSpec->maxBatchedItems();
    }

    bool OpenGLBatch::isSingleItemBatch() const
    {
        return batchSize() == 1;
    }

    void OpenGLBatch::upload()
    {
        calculateRequiredSizeOfBuffers();
        uploadVertices();
        uploadIndices();
        uploadUniforms();
    }

    void OpenGLBatch::uploadVertices()
    {
        m_VertexBuffer.bind();
        m_VertexBuffer.allocate(m_UploadMetadata.totalVertexBytes());

        char* buffer = reinterpret_cast<char*>(m_VertexBuffer.map(QOpenGLBuffer::WriteOnly));
        int size = m_VertexBuffer.size();
        int positionOffset = 0;
        int normalOffset = 0;
        int colorOffset = 0;
        int texCoordOffset = 0;

        foreach ( MatrixBatch* batch, m_MatrixBatches )
        {
            batch->copyVertexDataIntoBuffer(buffer, size, positionOffset, normalOffset, colorOffset, texCoordOffset);
        }

        m_VertexBuffer.unmap();
        m_VertexBuffer.release();
    }

    void OpenGLBatch::uploadIndices()
    {
        m_IndexBuffer.bind();
        m_IndexBuffer.allocate(m_UploadMetadata.m_iIndexBytes);

        char* buffer = reinterpret_cast<char*>(m_IndexBuffer.map(QOpenGLBuffer::WriteOnly));
        int size = m_IndexBuffer.size();
        int offset = 0;
        quint32 indexDelta = 0;

        foreach ( MatrixBatch* batch, m_MatrixBatches )
        {
            batch->copyIndexDataIntoBuffer(buffer, size, indexDelta, offset);
        }

        m_IndexBuffer.unmap();
        m_IndexBuffer.release();
    }

    void OpenGLBatch::uploadUniforms()
    {
        // Warning: The specific order of these commands may be important.
        // UBOs are finnicky.

        m_UniformBuffer.bind();
        m_UniformBuffer.allocate(m_pShaderSpec->maxBatchedItems() * 16 * sizeof(float));
        m_UniformBuffer.release();

        m_UniformBuffer.bindToIndex(ShaderDefs::LocalUniformBlockBindingPoint);

        m_UniformBuffer.bind();

        int offset = 0;
        foreach ( MatrixBatch* batch, m_MatrixBatches )
        {
            m_UniformBuffer.write(offset, batch->matrix().constData(), 16 * sizeof(float));
            offset += 16 * sizeof(float);
        }

        m_UniformBuffer.release();
    }

    int OpenGLBatch::matrixBatchCount() const
    {
        return m_MatrixBatches.count();
    }

    bool OpenGLBatch::matrixBatchLimitReached() const
    {
        return matrixBatchCount() >= batchSize();
    }

    int OpenGLBatch::createMatrixBatch(const QMatrix4x4 &mat)
    {
        Q_ASSERT_X(!matrixBatchLimitReached(), Q_FUNC_INFO, "No more space to add matrix batches!");

        if ( matrixBatchLimitReached() )
            return -1;

        m_MatrixBatches.append(new MatrixBatch(mat));
        return m_MatrixBatches.count() - 1;
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

    void OpenGLBatch::calculateRequiredSizeOfBuffers()
    {
        m_UploadMetadata.reset();

        foreach ( MatrixBatch* matBatch, m_MatrixBatches )
        {
            m_UploadMetadata += matBatch->buildItemMetadata();
        }
    }
}
