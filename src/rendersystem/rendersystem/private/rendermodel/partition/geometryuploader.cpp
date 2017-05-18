#include "geometryuploader.h"

#include "rendersystem/private/stores/openglshaderstore/openglshaderstore.h"

GeometryUploader::GeometryUploader(GeometryDataContainer &data, OpenGLBufferCollection &buffers)
    : m_GeometryDataContainer(data),
      m_OpenGLBuffers(buffers)
{

}

void GeometryUploader::uploadIfRequired(PrivateShaderDefs::ShaderId shaderId)
{
    quint32 flags = shouldUpload(shaderId);
    if ( flags == NoUploadFlags )
    {
        return;
    }

    // TODO: Upload
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

    if ( m_OpenGLBuffers.shaderId() == PrivateShaderDefs::UnknownShaderId)
    {
        return AllUploadFlags;
    }

    // Get the shaders and see whether the vertex format is the same.
    // Check all objects and see whether vertices/matrices are dirty.

    OpenGLShaderStore* shaderStore = OpenGLShaderStore::globalInstance();

    OpenGLShaderProgram* currentShader = shaderStore->object(m_OpenGLBuffers.shaderId());
    OpenGLShaderProgram* newShader = shaderStore->object(shaderId);

    if ( currentShader->vertexFormat() != newShader->vertexFormat() )
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
