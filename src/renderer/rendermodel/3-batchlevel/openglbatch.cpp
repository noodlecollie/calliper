#include "openglbatch.h"
#include "opengl/openglhelpers.h"

namespace
{
    void trySetAttributeBuffer(QOpenGLShaderProgram *shaderProgram,
                               int offsetInBytes,
                               NS_RENDERER::ShaderDefs::VertexArrayAttribute attribute,
                               int components)
    {
        if ( components > 0 )
        {
            shaderProgram->setAttributeBuffer(attribute,
                                                 GL_FLOAT,
                                                 offsetInBytes,
                                                 components);
        }
    }

    template<typename T>
    void readBufferData(QOpenGLBuffer &buffer, QVector<T> &out)
    {
        if ( !buffer.isCreated() )
            return;

        buffer.bind();
        out.resize(buffer.size() / sizeof(T));
        memcpy(out.data(), buffer.map(QOpenGLBuffer::ReadOnly), buffer.size());
        buffer.unmap();
    }
}

namespace NS_RENDERER
{
    OpenGLBatch::OpenGLBatch(QOpenGLBuffer::UsagePattern usagePattern, const IShaderSpec* shaderSpec)
        : m_iUsagePattern(usagePattern),
          m_bCreated(false),
          m_pShaderSpec(shaderSpec),
          m_iObjectIdMask(maskFromNumberOfBits(bitsRequired(m_pShaderSpec->maxBatchedItems()))),
          m_VertexBuffer(QOpenGLBuffer::VertexBuffer),
          m_IndexBuffer(QOpenGLBuffer::IndexBuffer),
          m_UniformBuffer(m_iUsagePattern),
          m_bNeedsUpload(true)
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

    void OpenGLBatch::setNeedsUpload(bool needsUpload)
    {
        m_bNeedsUpload = needsUpload;
    }

    bool OpenGLBatch::needsUpload() const
    {
        return m_bNeedsUpload;
    }

    void OpenGLBatch::uploadIfRequired()
    {
        if ( needsUpload() )
        {
            upload();
            setNeedsUpload(false);
        }
    }

    void OpenGLBatch::setVertexAttributes(QOpenGLShaderProgram *shaderProgram) const
    {
        int offset = 0;
        setAttributeBuffer(shaderProgram, ShaderDefs::PositionAttribute, offset);
        setAttributeBuffer(shaderProgram, ShaderDefs::NormalAttribute, offset);
        setAttributeBuffer(shaderProgram, ShaderDefs::ColorAttribute, offset);
        setAttributeBuffer(shaderProgram, ShaderDefs::TextureCoordinateAttribute, offset);
    }

    void OpenGLBatch::setAttributeBuffer(QOpenGLShaderProgram *shaderProgram, ShaderDefs::VertexArrayAttribute att, int &offsetInBytes) const
    {
        VertexFormat vertexFormat = m_pShaderSpec->vertexFormat();

        switch (att)
        {
            case ShaderDefs::PositionAttribute:
            {
                trySetAttributeBuffer(shaderProgram, offsetInBytes, att, vertexFormat.positionComponents());
                offsetInBytes += m_UploadMetadata.m_iPositionBytes;
                break;
            }

            case ShaderDefs::NormalAttribute:
            {
                trySetAttributeBuffer(shaderProgram, offsetInBytes, att, vertexFormat.normalComponents());
                offsetInBytes += m_UploadMetadata.m_iNormalBytes;
                break;
            }

            case ShaderDefs::ColorAttribute:
            {
                trySetAttributeBuffer(shaderProgram, offsetInBytes, att, vertexFormat.colorComponents());
                offsetInBytes += m_UploadMetadata.m_iColorBytes;
                break;
            }

            case ShaderDefs::TextureCoordinateAttribute:
            {
                trySetAttributeBuffer(shaderProgram, offsetInBytes, att, vertexFormat.textureCoordinateComponents());
                offsetInBytes += m_UploadMetadata.m_iTextureCoordinateBytes;
                break;
            }

            default:
            {
                Q_ASSERT_X(false, Q_FUNC_INFO, "Unrecognised attribute type!");
                break;
            }
        }
    }

    void OpenGLBatch::bindAll()
    {
        m_VertexBuffer.bind();
        m_IndexBuffer.bind();
        m_UniformBuffer.bind();
    }

    void OpenGLBatch::draw()
    {
        GL_CURRENT_F;
        f->glDrawElements(GL_TRIANGLES, m_UploadMetadata.m_iIndexBytes / sizeof(quint32), GL_UNSIGNED_INT, (void*)0);

        QVector<float> vData;
        exportVertexData(vData);

        QVector<quint32> iData;
        exportIndexData(iData);

        qDebug() << "Drew" << vData.count() << "vertex floats and" << iData.count() << "index ints.";
    }

    void OpenGLBatch::releaseAll()
    {
        m_VertexBuffer.release();
        m_IndexBuffer.release();
        m_UniformBuffer.release();
    }

    void OpenGLBatch::exportVertexData(QVector<float> &out)
    {
        readBufferData<float>(m_VertexBuffer, out);
    }

    void OpenGLBatch::exportIndexData(QVector<quint32> &out)
    {
        readBufferData<quint32>(m_IndexBuffer, out);
    }
}
