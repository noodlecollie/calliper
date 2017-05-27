#include "geometryrenderer.h"

#include "rendersystem/private/rendermodel/rendermodel-defs.h"

#include "rendersystem/private/stores/openglshaderstore/openglshaderstore.h"
#include "rendersystem/private/stores/materialstore/materialstore.h"
#include "rendersystem/private/stores/rendermodestore/rendermodestore.h"
#include "rendersystem/private/opengl/openglhelpers.h"

GeometryRenderer::GeometryRenderer(const RenderModelContext &context,
                                   RenderSystem::PublicStoreDefs::MaterialId materialId,
                                   GeometryOffsetTable &offsets,
                                   OpenGLBufferCollection &openGLBuffers)
    : m_Context(context),
      m_nMaterialId(materialId),
      m_OffsetTable(offsets),
      m_OpenGLBuffers(openGLBuffers),
      m_nItemsPerBatch(0),
      m_pCurrentShader(Q_NULLPTR),
      m_nDrawMode(GL_TRIANGLES)
{
    if ( m_nMaterialId == RenderSystem::PublicStoreDefs::INVALID_MATERIAL_ID )
    {
        return;
    }

    QSharedPointer<RenderSystem::RenderMaterial> material = MaterialStore::globalInstance()->object(m_nMaterialId);
    if ( !material )
    {
        return;
    }

    BaseRenderMode* renderMode = RenderModeStore::globalInstance()->object(m_Context.renderMode());
    if ( !renderMode )
    {
        return;
    }

    PrivateShaderDefs::ShaderId shaderId = renderMode->shaderId(material->shaderStyle());
    if ( shaderId == PrivateShaderDefs::UnknownShaderId )
    {
        return;
    }

    m_pCurrentShader = OpenGLShaderStore::globalInstance()->object(shaderId);
}

GLenum GeometryRenderer::drawMode() const
{
    return m_nDrawMode;
}

void GeometryRenderer::setDrawMode(GLenum mode)
{
    m_nDrawMode = mode;
}

void GeometryRenderer::draw()
{
    if ( !m_pCurrentShader || !m_OpenGLBuffers.isCreated() )
    {
        return;
    }

    m_nItemsPerBatch = m_pCurrentShader->maxBatchedItems();

    for ( int offsetItemBase = 0; offsetItemBase < m_OffsetTable.count(); offsetItemBase += m_nItemsPerBatch )
    {
        int lastItemInBatch = offsetItemBase + m_nItemsPerBatch - 1;
        if ( lastItemInBatch >= m_OffsetTable.count() )
        {
            lastItemInBatch = m_OffsetTable.count() - 1;
        }

        try
        {
            draw_x(offsetItemBase, lastItemInBatch);
        }
        catch (const InternalException& exception)
        {
            qCDebug(lcRenderModel) << "An error occurred during rendering:" << exception.error;
            break;
        }
    }

    releaseBuffers();
}

void GeometryRenderer::bindBuffers_x(int firstItem, int lastItem)
{
    if ( !m_OpenGLBuffers.vertexBuffer().bind() )
    {
        throw InternalException("Unable to bind vertex buffer.");
    }

    if ( !m_OpenGLBuffers.indexBuffer().bind() )
    {
        throw InternalException("Unable to bind index buffer.");
    }

    const quint32 uniformOffset = firstItem * SIZEOF_MATRIX_4X4;
    const quint32 uniformLength = (lastItem - firstItem + 1) * SIZEOF_MATRIX_4X4;

    if ( !m_OpenGLBuffers.uniformBuffer().bindRange(GL_UNIFORM_BUFFER, uniformOffset, uniformLength) )
    {
        throw InternalException("Unable to bind uniform buffer.");
    }
}

void GeometryRenderer::releaseBuffers()
{
    m_OpenGLBuffers.vertexBuffer().release();
    m_OpenGLBuffers.indexBuffer().release();
    m_OpenGLBuffers.uniformBuffer().release();
}

void GeometryRenderer::draw_x(int firstItem, int lastItem)
{
    quint32 beginOffsetInts = m_OffsetTable.at(firstItem).indexOffsetInts;

    const GeometryOffsetTable::ObjectOffsets endOffsets = m_OffsetTable.at(lastItem);
    quint32 endOffsetInts = endOffsets.indexOffsetInts + endOffsets.indexCountInts;

    quint32 indicesCount = endOffsetInts - beginOffsetInts;

    bindBuffers_x(firstItem, lastItem);

    GL_CURRENT_F;

    f->glDrawElements(m_nDrawMode, indicesCount, GL_UNSIGNED_INT, reinterpret_cast<GLvoid*>(beginOffsetInts));
}
