#include "geometryuploader.h"

#include "rendersystem/private/stores/openglshaderstore/openglshaderstore.h"

namespace
{
    const quint32 SIZEOF_MATRIX_4X4 = 4 * 4 * sizeof(float);

    inline quint32 maskFromNumberOfBits(int numBits)
    {
        return ~(static_cast<quint32>(0)) >> ((sizeof(quint32) * 8) - numBits);
    }

    inline quint8 bitsRequired(int maxValue)
    {
        for ( quint32 i = 0; i < sizeof(quint32) * 8; i++ )
        {
            if ( (1 << i) >= maxValue )
            {
                return i+1;
            }
        }

        return sizeof(quint32) * 8;
    }
}

GeometryUploader::GeometryUploader(GeometryDataContainer &data, OpenGLBufferCollection &buffers)
    : m_GeometryDataContainer(data),
      m_OpenGLBuffers(buffers),
      m_nCurrentShaderId(PrivateShaderDefs::UnknownShaderId),
      m_bShaderChangedSinceLastUpload(false),
      m_pCurrentShaderProgram(Q_NULLPTR),
      m_nCurrentObjectId(0),
      m_nCurrentObjectIdMask(0),
      m_nMaxBatchedItems(0),
      m_pUniformBufferData(Q_NULLPTR),
      m_pVertexBufferData(Q_NULLPTR),
      m_pIndexBufferData(Q_NULLPTR)
{

}

PrivateShaderDefs::ShaderId GeometryUploader::currentShaderId() const
{
    return m_nCurrentShaderId;
}

void GeometryUploader::setCurrentShaderId(PrivateShaderDefs::ShaderId shaderId)
{
    if ( m_nCurrentShaderId == shaderId )
    {
        return;
    }

    m_nCurrentShaderId = shaderId;
    m_bShaderChangedSinceLastUpload = true;

    if ( m_nCurrentShaderId == PrivateShaderDefs::UnknownShaderId )
    {
        m_pCurrentShaderProgram = Q_NULLPTR;
        m_nCurrentObjectIdMask = 0;
        m_nMaxBatchedItems = 0;
        return;
    }

    m_pCurrentShaderProgram = OpenGLShaderStore::globalInstance()->object(m_nCurrentShaderId);
    m_nMaxBatchedItems = m_pCurrentShaderProgram->maxBatchedItems();
    m_nCurrentObjectIdMask = maskFromNumberOfBits(bitsRequired(m_nMaxBatchedItems));
}

bool GeometryUploader::uploadIfRequired()
{
    if ( m_nCurrentShaderId == PrivateShaderDefs::UnknownShaderId )
    {
        return false;
    }

    quint32 flags = shouldUpload();
    if ( flags == NoUploadFlags )
    {
        // Nothing to upload, so all is fine.
        return true;
    }

    if ( (flags & MatricesUploadFlag) == MatricesUploadFlag && !uploadAllMatrices() )
    {
        return false;
    }

    if ( (flags & VerticesUploadFlag) == VerticesUploadFlag && !uploadAllVertexData() )
    {
        return false;
    }

    m_bShaderChangedSinceLastUpload = false;
    return true;
}

quint32 GeometryUploader::shouldUpload() const
{
    if ( m_bShaderChangedSinceLastUpload )
    {
        return AllUploadFlags;
    }

    return checkForDirtyGeometry();
}

quint32 GeometryUploader::checkForDirtyGeometry() const
{
    quint32 flags = NoUploadFlags;

    for ( GeometryDataContainer::Iterator it = m_GeometryDataContainer.begin();
          it != m_GeometryDataContainer.end();
          ++it )
    {
        QSharedPointer<GeometryData> data = it.value();

        if ( data->verticesDirty() )
        {
            flags |= VerticesUploadFlag;
        }

        if ( data->matrixDirty() )
        {
            flags |= MatricesUploadFlag;
        }

        if ( flags == AllUploadFlags )
        {
            break;
        }
    }

    return flags;
}

bool GeometryUploader::uploadAllMatrices()
{
    if ( !ensureBuffersCreated() )
    {
        return false;
    }

    bool success = true;

    try
    {
        prepareUniformBufferForUpload_x();

        for ( GeometryDataContainer::ConstIterator it = m_GeometryDataContainer.constBegin();
              it != m_GeometryDataContainer.constEnd();
              ++it )
        {
            memcpy(m_pUniformBufferData, it.value()->modelToWorldMatrix().constData(), SIZEOF_MATRIX_4X4);
            m_pUniformBufferData += SIZEOF_MATRIX_4X4;
        }
    }
    catch (const InternalException&)
    {
        success = false;
    }

    releaseUniformBuffer();
    return success;
}

void GeometryUploader::prepareUniformBufferForUpload_x()
{
    // The order of these may be important. UBOs are awkward.
    if ( !m_OpenGLBuffers.uniformBuffer().bind() ||
         !m_OpenGLBuffers.uniformBuffer().allocate(m_GeometryDataContainer.count() * SIZEOF_MATRIX_4X4) ||
         !m_OpenGLBuffers.uniformBuffer().release() ||
         !m_OpenGLBuffers.uniformBuffer().bindToIndex(PrivateShaderDefs::LocalUniformBlockBindingPoint) )
    {
        throw InternalException();
    }

    m_pUniformBufferData = static_cast<char*>(m_OpenGLBuffers.uniformBuffer().map(QOpenGLBuffer::WriteOnly));
    if ( !m_pUniformBufferData )
    {
        throw InternalException();
    }
}

void GeometryUploader::releaseUniformBuffer()
{
    m_pUniformBufferData = Q_NULLPTR;
    m_OpenGLBuffers.uniformBuffer().unmap();
    m_OpenGLBuffers.uniformBuffer().release();
}

bool GeometryUploader::uploadAllVertexData()
{
    if ( !ensureBuffersCreated() )
    {
        return false;
    }

    bool success = true;

    try
    {
        prepareVertexBufferForUpload_x();
        prepareIndexBufferForUpload_x();
        m_nCurrentObjectId = 0;

        for ( GeometryDataContainer::ConstIterator it = m_GeometryDataContainer.constBegin();
              it != m_GeometryDataContainer.constEnd();
              ++it )
        {
            // TODO
        }
    }
    catch (const InternalException&)
    {
        success = false;
    }

    releaseVertexBuffer();
    releaseIndexBuffer();
    return success;
}

void GeometryUploader::prepareVertexBufferForUpload_x()
{
    if ( !m_OpenGLBuffers.vertexBuffer().bind() )
    {
        throw InternalException();
    }

    m_OpenGLBuffers.vertexBuffer().allocate(computeTotalRequiredVertexBytes());
    m_pVertexBufferData = static_cast<float*>(m_OpenGLBuffers.vertexBuffer().map(QOpenGLBuffer::WriteOnly));

    if ( !m_pVertexBufferData )
    {
        throw InternalException();
    }
}

void GeometryUploader::releaseVertexBuffer()
{
    m_OpenGLBuffers.vertexBuffer().unmap();
    m_OpenGLBuffers.vertexBuffer().release();
}

void GeometryUploader::prepareIndexBufferForUpload_x()
{
    if ( !m_OpenGLBuffers.indexBuffer().bind() )
    {
        throw InternalException();
    }

    m_OpenGLBuffers.indexBuffer().allocate(computeTotalRequiredIndexBytes());
    m_pIndexBufferData = static_cast<quint32*>(m_OpenGLBuffers.indexBuffer().map(QOpenGLBuffer::WriteOnly));

    if ( !m_pIndexBufferData )
    {
        throw InternalException();
    }
}

void GeometryUploader::releaseIndexBuffer()
{
    m_OpenGLBuffers.indexBuffer().unmap();
    m_OpenGLBuffers.indexBuffer().release();
}

bool GeometryUploader::ensureBuffersCreated()
{
    return m_OpenGLBuffers.isCreated() || m_OpenGLBuffers.create();
}

quint32 GeometryUploader::computeTotalRequiredVertexBytes()
{
    if ( !m_pCurrentShaderProgram )
    {
        return 0;
    }

    quint32 totalBytes = 0;
    VertexFormat vertexFormat = m_pCurrentShaderProgram->vertexFormat();

    for ( GeometryDataContainer::ConstIterator it = m_GeometryDataContainer.constBegin();
          it != m_GeometryDataContainer.constEnd();
          ++it )
    {
        totalBytes += it.value()->computeTotalVertexBytes(vertexFormat);
    }

    return totalBytes;
}

quint32 GeometryUploader::computeTotalRequiredIndexBytes()
{
    quint32 totalBytes = 0;

    for ( GeometryDataContainer::ConstIterator it = m_GeometryDataContainer.constBegin();
          it != m_GeometryDataContainer.constEnd();
          ++it )
    {
        totalBytes += it.value()->computeTotalIndexBytes();
    }

    return totalBytes;
}

void GeometryUploader::consolidateVertexData(QSharedPointer<GeometryData>& geometry, QVector<float>& vertexData)
{
    if ( !m_pCurrentShaderProgram )
    {
        return;
    }

    VertexFormat vertexFormat = m_pCurrentShaderProgram->vertexFormat();
    int totalVertexComponents = vertexFormat.totalVertexComponents();

    m_nCurrentObjectId = (m_nCurrentObjectId + 1) % m_nMaxBatchedItems;

    quint32 totalVertexDataBytes = geometry->computeTotalVertexBytes(vertexFormat);

    vertexData.resize(totalVertexDataBytes);

    int beginOffset = 0;

    consolidate(geometry->positions(),
                vertexData,
                vertexFormat.positionComponents(),
                beginOffset,
                totalVertexComponents);

    beginOffset += vertexFormat.positionComponents();

    consolidate(geometry->normals(),
                vertexData,
                vertexFormat.normalComponents(),
                beginOffset,
                totalVertexComponents);

    beginOffset += vertexFormat.normalComponents();

    consolidate(geometry->colors(),
                vertexData,
                vertexFormat.colorComponents(),
                beginOffset,
                totalVertexComponents);

    beginOffset += vertexFormat.colorComponents();

    consolidate(geometry->textureCoordinates(),
                vertexData,
                vertexFormat.textureCoordinateComponents(),
                beginOffset,
                totalVertexComponents);
}

void GeometryUploader::consolidateIndexData(QSharedPointer<GeometryData>& geometry, QVector<quint32>& indexData)
{
    static_assert(false, "Complete this function");
}

void GeometryUploader::consolidate(const QVector<QVector4D> &source,
                                   QVector<float> &destination,
                                   int components,
                                   int offset,
                                   int stride)
{
    if ( stride <= 0 )
    {
        stride = 1;
    }

    for ( int srcItem = 0, destItem = offset;
          srcItem < source.count();
          ++srcItem, destItem += stride)
    {
        const QVector4D& vec = source[srcItem];

        for ( int curComponent = 0; curComponent < components; ++curComponent )
        {
            float value = vec[curComponent];

            if ( curComponent == components - 1 )
            {
                quint32* valueAsInt = reinterpret_cast<quint32*>(&value);
                *valueAsInt |= (static_cast<quint32>(m_nCurrentObjectId) & m_nCurrentObjectIdMask);
            }

            destination[destItem + curComponent] = value;
        }
    }
}
