#include "geometryuploader.h"

#include "rendersystem/private/stores/openglshaderstore/openglshaderstore.h"
#include "geometryconsolidator.h"

namespace
{
    const quint32 SIZEOF_MATRIX_4X4 = 4 * 4 * sizeof(float);
}

GeometryUploader::GeometryUploader(GeometryDataContainer &data, OpenGLBufferCollection &buffers)
    : m_GeometryDataContainer(data),
      m_OpenGLBuffers(buffers),
      m_nCurrentShaderId(PrivateShaderDefs::UnknownShaderId),
      m_bShaderChangedSinceLastUpload(false),
      m_pCurrentShaderProgram(Q_NULLPTR),
      m_Consolidator(m_GeometryDataContainer, m_pCurrentShaderProgram),
      m_pUniformBufferData(Q_NULLPTR)
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

    setUpShaderDependentMembers();
}

void GeometryUploader::setUpShaderDependentMembers()
{
    if ( m_nCurrentShaderId == PrivateShaderDefs::UnknownShaderId )
    {
        m_pCurrentShaderProgram = Q_NULLPTR;
        return;
    }

    m_pCurrentShaderProgram = OpenGLShaderStore::globalInstance()->object(m_nCurrentShaderId);
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

    consolidateVerticesAndIndices();

    bool success = true;

    try
    {
        uploadVertices_x();
        uploadIndices_x();
    }
    catch (const InternalException&)
    {
        success = false;
    }

    releaseVertexBuffer();
    releaseIndexBuffer();
    return success;
}

void GeometryUploader::uploadVertices_x()
{
    if ( !m_OpenGLBuffers.vertexBuffer().bind() )
    {
        throw InternalException();
    }

    m_OpenGLBuffers.vertexBuffer().allocate(m_Consolidator.vertexData().constBegin(), m_Consolidator.vertexData().count() * sizeof(float));
}

void GeometryUploader::releaseVertexBuffer()
{
    m_OpenGLBuffers.vertexBuffer().unmap();
    m_OpenGLBuffers.vertexBuffer().release();
}

void GeometryUploader::uploadIndices_x()
{
    if ( !m_OpenGLBuffers.indexBuffer().bind() )
    {
        throw InternalException();
    }

    m_OpenGLBuffers.indexBuffer().allocate(m_Consolidator.indexData().constBegin(), m_Consolidator.indexData().count() * sizeof(quint32));
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

void GeometryUploader::consolidateVerticesAndIndices()
{
    m_Consolidator.setShader(m_pCurrentShaderProgram);
    m_Consolidator.consolidate();
}
