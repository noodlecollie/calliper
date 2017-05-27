#include "geometryuploader.h"

#include "rendersystem/private/stores/openglshaderstore/openglshaderstore.h"
#include "rendersystem/private/stores/rendermodestore/rendermodestore.h"
#include "rendersystem/private/stores/materialstore/materialstore.h"

#include "rendersystem/private/opengl/openglhelpers.h"
#include "geometryconsolidator.h"

GeometryUploader::GeometryUploader(const RenderModelContext &context,
                                   RenderSystem::PublicStoreDefs::MaterialId materialId,
                                   GeometryDataContainer &data,
                                   GeometryOffsetTable &offsetTable,
                                   OpenGLBufferCollection &buffers)
    : m_Context(context),
      m_nMaterialId(materialId),
      m_GeometryDataContainer(data),
      m_OffsetTable(offsetTable),
      m_OpenGLBuffers(buffers),
      m_pCurrentShaderProgram(Q_NULLPTR),
      m_nShaderId(PrivateShaderDefs::UnknownShaderId),
      m_nShaderIdWhenLastUploaded(PrivateShaderDefs::UnknownShaderId),
      m_Consolidator(m_Context, m_nMaterialId, m_GeometryDataContainer, m_OffsetTable),
      m_pUniformBufferData(Q_NULLPTR)
{

}

bool GeometryUploader::uploadIfRequired()
{
    getShaderFromMaterial();

    if ( !isValid() )
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

    m_nShaderIdWhenLastUploaded = m_nShaderId;
    m_pCurrentShaderProgram = Q_NULLPTR;
    return true;
}

void GeometryUploader::getShaderFromMaterial()
{
    m_pCurrentShaderProgram = Q_NULLPTR;

    if ( m_Context.renderMode() == RenderSystem::PublicShaderDefs::UnknownRenderMode )
    {
        return;
    }

    QSharedPointer<RenderSystem::RenderMaterial> material = MaterialStore::globalInstance()->object(m_nMaterialId);
    if ( !material )
    {
        return;
    }

    BaseRenderMode* mode = RenderModeStore::globalInstance()->object(m_Context.renderMode());
    if ( !mode )
    {
        return;
    }

    m_nShaderId = mode->shaderId(material->shaderStyle());
    if ( m_nShaderId == PrivateShaderDefs::UnknownShaderId )
    {
        return;
    }

    m_pCurrentShaderProgram = OpenGLShaderStore::globalInstance()->object(m_nShaderId);
}

bool GeometryUploader::isValid() const
{
    return m_nShaderId != PrivateShaderDefs::UnknownShaderId;
}

quint32 GeometryUploader::shouldUpload() const
{
    if ( m_nShaderId == PrivateShaderDefs::UnknownShaderId )
    {
        return NoUploadFlags;
    }

    if ( m_nShaderId != m_nShaderIdWhenLastUploaded )
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
    m_Consolidator.consolidate();
}
