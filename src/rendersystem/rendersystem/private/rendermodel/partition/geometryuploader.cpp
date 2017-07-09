#include "geometryuploader.h"

#include "geometryconsolidator.h"
#include "renderutils.h"

#include "rendersystem/private/static-stores/openglshaderstore/openglshaderstore.h"
#include "rendersystem/private/opengl/uniforms/std140alignment.h"

#include "calliperutil/opengl/openglhelpers.h"

namespace
{
    int getNumComponents(const VertexFormat& format, PrivateShaderDefs::VertexArrayAttribute attribute)
    {
        switch ( attribute )
        {
            case PrivateShaderDefs::PositionAttribute:
            {
                return format.positionComponents();
            }

            case PrivateShaderDefs::NormalAttribute:
            {
                return format.normalComponents();
            }

            case PrivateShaderDefs::ColorAttribute:
            {
                return format.colorComponents();
            }

            case PrivateShaderDefs::TextureCoordinateAttribute:
            {
                return format.textureCoordinateComponents();
            }

            default:
            {
                Q_ASSERT_X(false, Q_FUNC_INFO, "Unrecognised attribute!");
                return 0;
            }
        }
    }

    int computeOffsetInBytes(const VertexFormat& format, PrivateShaderDefs::VertexArrayAttribute attribute)
    {
        int offset = 0;
        if ( attribute == PrivateShaderDefs::PositionAttribute )
        {
            return offset;
        }

        offset += format.positionComponents() * sizeof(float);
        if ( attribute == PrivateShaderDefs::NormalAttribute )
        {
            return offset;
        }

        offset += format.normalComponents() * sizeof(float);
        if ( attribute == PrivateShaderDefs::ColorAttribute )
        {
            return offset;
        }

        offset += format.colorComponents() * sizeof(float);
        if ( attribute == PrivateShaderDefs::TextureCoordinateAttribute )
        {
            return offset;
        }

        Q_ASSERT_X(false, Q_FUNC_INFO, "Unrecognised attribute type!");
        return 0;
    }

    void trySetAttributeBuffer(OpenGLShaderProgram& shaderProgram,
                               PrivateShaderDefs::VertexArrayAttribute attribute)
    {
        const int components = getNumComponents(shaderProgram.vertexFormat(), attribute);

        if ( components > 0 )
        {
            shaderProgram.setAttributeBuffer(attribute,
                                             GL_FLOAT,
                                             computeOffsetInBytes(shaderProgram.vertexFormat(), attribute),
                                             components,
                                             shaderProgram.vertexFormat().totalVertexComponents() * sizeof(float));
        }
    }
}

GeometryUploader::GeometryUploader(const RenderModelContext &context,
                                   RenderSystem::MaterialDefs::MaterialId materialId,
                                   GeometryDataContainer &data,
                                   GeometryOffsetTable &offsetTable,
                                   UniformBatchTable& batchTable,
                                   OpenGLBufferCollection &buffers)
    : m_Context(context),
      m_nMaterialId(materialId),
      m_GeometryDataContainer(data),
      m_OffsetTable(offsetTable),
      m_BatchTable(batchTable),
      m_OpenGLBuffers(buffers),
      m_pCurrentShaderProgram(Q_NULLPTR),
      m_nShaderId(PrivateShaderDefs::UnknownShaderId),
      m_nShaderIdWhenLastUploaded(PrivateShaderDefs::UnknownShaderId),
      m_Consolidator(m_Context, m_nMaterialId, m_GeometryDataContainer, m_OffsetTable),
      m_BatchGenerator(m_GeometryDataContainer),
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

    m_OpenGLBuffers.vertexArrayObject().bind();
    m_pCurrentShaderProgram->bindFull();

    bindShaderAttributesToVAO();
    generateBatches();

    if ( (flags & MatricesUploadFlag) == MatricesUploadFlag && !uploadAllUniforms() )
    {
        m_OpenGLBuffers.vertexArrayObject().release();
        m_pCurrentShaderProgram = Q_NULLPTR;
        return false;
    }

    if ( (flags & VerticesUploadFlag) == VerticesUploadFlag && !uploadAllVertexData() )
    {
        m_OpenGLBuffers.vertexArrayObject().release();
        m_pCurrentShaderProgram = Q_NULLPTR;
        return false;
    }

    m_pCurrentShaderProgram->releaseFull();
    m_OpenGLBuffers.vertexArrayObject().release();

    m_nShaderIdWhenLastUploaded = m_nShaderId;
    m_pCurrentShaderProgram = Q_NULLPTR;
    return true;
}

void GeometryUploader::getShaderFromMaterial()
{
    m_nShaderId = RenderUtils::shaderFromMaterial(m_Context.renderMode(), m_nMaterialId);

    if ( m_nShaderId != PrivateShaderDefs::UnknownShaderId )
    {
        m_pCurrentShaderProgram = OpenGLShaderStore::globalInstance()->object(m_nShaderId);
    }
    else
    {
        m_pCurrentShaderProgram = Q_NULLPTR;
    }
}

bool GeometryUploader::isValid() const
{
    return m_nShaderId != PrivateShaderDefs::UnknownShaderId && m_pCurrentShaderProgram != Q_NULLPTR;
}

quint32 GeometryUploader::shouldUpload() const
{
    if ( m_nShaderId == PrivateShaderDefs::UnknownShaderId || !m_pCurrentShaderProgram )
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

void GeometryUploader::generateBatches()
{
    m_BatchGenerator.buildBatches(m_pCurrentShaderProgram->maxBatchedItems());
}

bool GeometryUploader::uploadAllUniforms()
{
    if ( !buffersCreated() )
    {
        return false;
    }

    bool success = true;

    try
    {
        prepareUniformBufferForUpload_x();
        uploadUniformsInBatches();
    }
    catch (const InternalException&)
    {
        success = false;
    }

    releaseUniformBuffer();
    return success;
}

quint32 GeometryUploader::calculateRequiredUniformBufferSize() const
{
    return m_BatchGenerator.batchCount() * calculateBatchSize();
}

quint32 GeometryUploader::calculateBatchSize() const
{
    return m_pCurrentShaderProgram->maxBatchedItems() * UniformStd140::baseAlignmentArray<QMatrix4x4>().baseAlignment;
}

void GeometryUploader::uploadUniformsInBatches()
{
    quint32 accumulatedSize = 0;
    quint32 currentGeometryItem = 0;

    for ( int batchIndex = 0; batchIndex < m_BatchGenerator.batchCount(); ++batchIndex )
    {
        const BatchGenerator::GeometryDataVector& batch = m_BatchGenerator.batch(batchIndex);
        const quint32 batchSize = calculateBatchSize();

        char* batchDataPointer = m_pUniformBufferData;
        for ( int geometryDataIndex = 0; geometryDataIndex < batch.count(); ++geometryDataIndex )
        {
            const BatchGenerator::GeometryDataPointer& geometryData = batch.at(geometryDataIndex);
            memcpy(batchDataPointer, geometryData->modelToWorldMatrix().constData(), SIZEOF_MATRIX_4X4);
            batchDataPointer += SIZEOF_MATRIX_4X4;
        }

        UniformBatchTable::UniformBatchOffsets& offsets = m_BatchTable.createNewItem();

        offsets.batchOffsetBytes = accumulatedSize;
        offsets.batchSizeBytes = batchSize;
        offsets.firstGeometryItem = currentGeometryItem;
        offsets.lastGeometryItem = currentGeometryItem + batch.count() - 1;

        m_pUniformBufferData += batchSize;
        accumulatedSize += batchSize;
        currentGeometryItem += batch.count();
    }
}

void GeometryUploader::prepareUniformBufferForUpload_x()
{
    // The order of these may be important. UBOs are awkward.
    if ( !m_OpenGLBuffers.uniformBuffer().bind() ||
         !m_OpenGLBuffers.uniformBuffer().allocate(calculateRequiredUniformBufferSize()) ||
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
    if ( !buffersCreated() )
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

    return success;
}

void GeometryUploader::uploadVertices_x()
{
    if ( !m_OpenGLBuffers.vertexBuffer().bind() )
    {
        throw InternalException();
    }

    m_OpenGLBuffers.vertexBuffer().allocate(m_Consolidator.vertexData().constBegin(), m_Consolidator.vertexData().count() * sizeof(float));
    m_OpenGLBuffers.vertexBuffer().release();
}

void GeometryUploader::uploadIndices_x()
{
    if ( !m_OpenGLBuffers.indexBuffer().bind() )
    {
        throw InternalException();
    }

    m_OpenGLBuffers.indexBuffer().allocate(m_Consolidator.indexData().constBegin(), m_Consolidator.indexData().count() * sizeof(quint32));
    m_OpenGLBuffers.indexBuffer().release();
}

bool GeometryUploader::buffersCreated()
{
    return m_OpenGLBuffers.isCreated() || m_OpenGLBuffers.create();
}

void GeometryUploader::consolidateVerticesAndIndices()
{
    m_Consolidator.consolidate();
}

void GeometryUploader::bindShaderAttributesToVAO()
{
    if ( !m_pCurrentShaderProgram )
    {
        return;
    }

    trySetAttributeBuffer(*m_pCurrentShaderProgram, PrivateShaderDefs::PositionAttribute);
    trySetAttributeBuffer(*m_pCurrentShaderProgram, PrivateShaderDefs::NormalAttribute);
    trySetAttributeBuffer(*m_pCurrentShaderProgram, PrivateShaderDefs::ColorAttribute);
    trySetAttributeBuffer(*m_pCurrentShaderProgram, PrivateShaderDefs::TextureCoordinateAttribute);
}
