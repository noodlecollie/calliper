#include "geometryuploader.h"

#include "rendersystem/private/stores/openglshaderstore/openglshaderstore.h"

GeometryUploader::GeometryUploader(GeometryDataContainer &data, OpenGLBufferCollection &buffers)
    : m_GeometryDataContainer(data),
      m_OpenGLBuffers(buffers),
      m_nLastShaderId(PrivateShaderDefs::UnknownShaderId)
{

}

bool GeometryUploader::uploadIfRequired(PrivateShaderDefs::ShaderId shaderId)
{
    if ( shaderId == PrivateShaderDefs::UnknownShaderId )
    {
        return false;
    }

    quint32 flags = shouldUpload(shaderId);
    if ( flags == NoUploadFlags )
    {
        return;
    }

    m_nLastShaderId = shaderId;

    if ( (flags & MatricesUploadFlag) == MatricesUploadFlag && !uploadAllMatrices() )
    {
        return false;
    }

    if ( (flag & VerticesUploadFlag) == VerticesUploadFlag && !uploadAllVertexData() )
    {
        return false;
    }

    return true;
}

quint32 GeometryUploader::shouldUpload(PrivateShaderDefs::ShaderId shaderId) const
{
    // No need to upload if all of the following are true:
    // - No matrices changed
    // - No vertex data changed
    // - Shader is the same (and not unknown).

    if ( shaderId == PrivateShaderDefs::UnknownShaderId )
    {
        return NoUploadFlags;
    }

    if ( m_nLastShaderId == PrivateShaderDefs::UnknownShaderId)
    {
        return AllUploadFlags;
    }

    // Get the shaders and see whether the vertex format is the same.
    // Check all objects and see whether vertices/matrices are dirty.

    OpenGLShaderStore* shaderStore = OpenGLShaderStore::globalInstance();

    OpenGLShaderProgram* lastShader = shaderStore->object(m_nLastShaderId);
    OpenGLShaderProgram* newShader = shaderStore->object(shaderId);

    if ( lastShader->vertexFormat() != newShader->vertexFormat() )
    {
        return AllUploadFlags;
    }

    return checkForDirtyGeometry();
}

bool GeometryUploader::uploadAllMatrices()
{
    if ( !m_OpenGLBuffers.isCreated() && !m_OpenGLBuffers.create() )
    {
        return false;
    }

    m_OpenGLBuffers.uniformBuffer().bind();

    m_OpenGLBuffers.uniformBuffer().release();
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
