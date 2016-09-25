#include "openglbatch.h"
#include "opengl/openglhelpers.h"
#include "opengl/openglerrors.h"

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
        buffer.release();
    }

    template<typename T>
    void readBufferData(NS_RENDERER::OpenGLUniformBuffer &buffer, QVector<T> &out)
    {
        if ( !buffer.isCreated() )
            return;

        buffer.bind();
        out.resize(buffer.size() / sizeof(T));
        memcpy(out.data(), buffer.map(QOpenGLBuffer::ReadOnly), buffer.size());
        buffer.unmap();
        buffer.release();
    }
}

namespace NS_RENDERER
{
    OpenGLBatch::OpenGLBatch(QOpenGLBuffer::UsagePattern usagePattern, const IShaderSpec* shaderSpec)
        : m_iUsagePattern(usagePattern),
          m_bCreated(false),
          m_pShaderSpec(shaderSpec),
          m_iMaskBitsRequired(bitsRequired(m_pShaderSpec->maxBatchedItems())),
          m_iObjectIdMask(maskFromNumberOfBits(m_iMaskBitsRequired)),
          m_VertexBuffer(QOpenGLBuffer::VertexBuffer),
          m_IndexBuffer(QOpenGLBuffer::IndexBuffer),
          m_UniformBuffer(m_iUsagePattern),
          m_bNeedsUpload(true)
    {
        Q_ASSERT(shaderSpec);

        GLTRY(m_VertexBuffer.setUsagePattern(m_iUsagePattern));
        GLTRY(m_IndexBuffer.setUsagePattern(m_iUsagePattern));
        GLTRY(m_UniformBuffer.setUsagePattern(m_iUsagePattern));
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

        m_bCreated = true;
        GLTRY(m_bCreated = m_bCreated && m_VertexBuffer.create());
        GLTRY(m_bCreated = m_bCreated && m_IndexBuffer.create());
        GLTRY(m_bCreated = m_bCreated && m_UniformBuffer.create());
    }

    void OpenGLBatch::destroy()
    {
        if ( !m_bCreated )
            return;

        GLTRY(m_VertexBuffer.destroy());
        GLTRY(m_IndexBuffer.destroy());
        GLTRY(m_UniformBuffer.destroy());

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
        bool successfulBind = false;
        GLTRY(successfulBind = m_VertexBuffer.bind());
        Q_ASSERT_X(successfulBind, Q_FUNC_INFO, "Could not bind vertex buffer");

        GLTRY(m_VertexBuffer.allocate(m_UploadMetadata.totalVertexBytes()));

        void* vBuffer = NULL;
        GLTRY(vBuffer = m_VertexBuffer.map(QOpenGLBuffer::WriteOnly));
        char* buffer = reinterpret_cast<char*>(vBuffer);

        int size = 0;
        GLTRY(size = m_VertexBuffer.size());

        copyAllVertexData(buffer, size);

        GLTRY(m_VertexBuffer.unmap());
        GLTRY(m_VertexBuffer.release());
    }

    void OpenGLBatch::copyAllVertexData(char* buffer, int size)
    {
        int offset = 0;

        int positionOffset = offset;
        offset += m_UploadMetadata.m_iPositionBytes;

        int normalOffset = offset;
        offset += m_UploadMetadata.m_iNormalBytes;

        int colorOffset = offset;
        offset += m_UploadMetadata.m_iColorBytes;

        int texCoordOffset = offset;
        offset += m_UploadMetadata.m_iTextureCoordinateBytes;

        quint32 id = 0;
        foreach ( MatrixBatch* batch, m_MatrixBatches )
        {
            int oldPositionOffset = positionOffset;
            batch->copyVertexDataIntoBuffer(buffer, size, positionOffset, normalOffset, colorOffset, texCoordOffset);
            updateObjectIds(buffer, oldPositionOffset, positionOffset - oldPositionOffset, id);
            id++;
        }
    }

    void OpenGLBatch::updateObjectIds(char *buffer, int offset, int numBytes, quint32 id)
    {
        int numFloats = numBytes / sizeof(float);
        float* floatData = reinterpret_cast<float*>(buffer);
        int numComponents = m_pShaderSpec->vertexFormat().positionComponents();
        int floatOffset = offset / sizeof(float);

        for ( int i = floatOffset; i < floatOffset + numFloats; i++ )
        {
            // Fast forward past the first components.
            i += numComponents - 1;

            // Set the last component using a mask.
            // We bit shift down and then up again to set the
            // lower bits to zero, then OR with the masked ID.
            quint32 valAsInt = static_cast<quint32>(floatData[i]);
            valAsInt = ((valAsInt >> m_iMaskBitsRequired) << m_iMaskBitsRequired) | (id & m_iObjectIdMask);
            float f = static_cast<float>(valAsInt);
            floatData[i] = f;
        }
    }

    void OpenGLBatch::uploadIndices()
    {
        bool successfulBind = false;
        GLTRY(successfulBind = m_IndexBuffer.bind());
        Q_ASSERT_X(successfulBind, Q_FUNC_INFO, "Could not bind index buffer");

        GLTRY(m_IndexBuffer.allocate(m_UploadMetadata.m_iIndexBytes));

        void* vBuffer = NULL;
        GLTRY(vBuffer = m_IndexBuffer.map(QOpenGLBuffer::WriteOnly));
        char* buffer = reinterpret_cast<char*>(vBuffer);

        int size = 0;
        GLTRY(size = m_IndexBuffer.size());

        int offset = 0;
        quint32 indexDelta = 0;

        foreach ( MatrixBatch* batch, m_MatrixBatches )
        {
            batch->copyIndexDataIntoBuffer(buffer, size, indexDelta, offset);
        }

        GLTRY(m_IndexBuffer.unmap());
        GLTRY(m_IndexBuffer.release());
    }

    void OpenGLBatch::uploadUniforms()
    {
        // Warning: The specific order of these commands may be important.
        // UBOs are finnicky.

        GLTRY(m_UniformBuffer.bind());
        GLTRY(m_UniformBuffer.allocate(m_pShaderSpec->maxBatchedItems() * 16 * sizeof(float)));
        GLTRY(m_UniformBuffer.release());

        GLTRY(m_UniformBuffer.bindToIndex(ShaderDefs::LocalUniformBlockBindingPoint));

        int offset = 0;
        foreach ( MatrixBatch* batch, m_MatrixBatches )
        {
            GLTRY(m_UniformBuffer.write(offset, batch->matrix().constData(), 16 * sizeof(float)));
            offset += 16 * sizeof(float);
        }

        GLTRY(m_UniformBuffer.release());
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
        GLTRY(m_VertexBuffer.bind());
        GLTRY(m_IndexBuffer.bind());
        GLTRY(m_UniformBuffer.bindToIndex(ShaderDefs::LocalUniformBlockBindingPoint));
    }

    void OpenGLBatch::draw()
    {
        GL_CURRENT_F;
        GLTRY(f->glDrawElements(GL_TRIANGLES, m_UploadMetadata.m_iIndexBytes / sizeof(quint32), GL_UNSIGNED_INT, (void*)0));
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

    void OpenGLBatch::exportUniformData(QVector<float> &out)
    {
        readBufferData<float>(m_UniformBuffer, out);
    }
}
