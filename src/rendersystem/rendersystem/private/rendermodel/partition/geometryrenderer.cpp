#include "geometryrenderer.h"

#include "renderutils.h"

#include "rendersystem/private/rendermodel/rendermodel-defs.h"
#include "rendersystem/private/static-stores/openglshaderstore/openglshaderstore.h"

#include "calliperutil/opengl/openglhelpers.h"
#include "calliperutil/opengl/openglerrors.h"

namespace
{
    inline int itemsInRange(int firstItem, int lastItem)
    {
        return lastItem - firstItem + 1;
    }
}

GeometryRenderer::GeometryRenderer(const RenderModelContext &context,
                                   RenderSystem::MaterialDefs::MaterialId materialId,
                                   const GeometryOffsetTable &offsets,
                                   const UniformBatchTable& batchTable,
                                   OpenGLBufferCollection &openGLBuffers)
    : m_Context(context),
      m_nMaterialId(materialId),
      m_OffsetTable(offsets),
      m_BatchTable(batchTable),
      m_OpenGLBuffers(openGLBuffers),
      m_nItemsPerBatch(0),
      m_pCurrentShader(Q_NULLPTR),
      m_nDrawMode(RenderSystem::GeometrySection::DrawMode::DrawTriangles),
      m_flLineWidth(1.0f)
{
    const PrivateShaderDefs::ShaderId shaderId = RenderUtils::shaderFromMaterial(m_Context.renderMode(), m_nMaterialId);

    if ( shaderId != PrivateShaderDefs::UnknownShaderId )
    {
        m_pCurrentShader = OpenGLShaderStore::globalInstance()->object(shaderId);
    }
    else
    {
        m_pCurrentShader = Q_NULLPTR;
    }
}

int GeometryRenderer::getLastItemForNextDraw(int firstItem) const
{
    if ( firstItem + 1 >= m_OffsetTable.count() )
    {
        return firstItem;
    }

    const RenderSystem::GeometrySection::DrawMode firstItemDrawMode = m_OffsetTable[firstItem].drawMode;
    const float firstItemLineWidth = m_OffsetTable[firstItem].lineWidth;

    for ( int lastItem = firstItem + 1;
          itemsInRange(firstItem, lastItem) <= m_nItemsPerBatch && lastItem < m_OffsetTable.count();
          ++lastItem )
    {
        // If draw params are different on this item, the last item
        // should be the previous one.
        if ( m_OffsetTable[lastItem].drawMode != firstItemDrawMode ||
             m_OffsetTable[lastItem].lineWidth != firstItemLineWidth )
        {
            return lastItem - 1;
        }

        // If this item is the last in the table, don't go any further.
        if ( lastItem == m_OffsetTable.count() - 1 )
        {
            return lastItem;
        }
    }

    return firstItem + m_nItemsPerBatch - 1;
}

void GeometryRenderer::draw()
{
    if ( !m_pCurrentShader || !m_OpenGLBuffers.isCreated() )
    {
        return;
    }

    m_pCurrentShader->bindWithUniforms();
    m_OpenGLBuffers.vertexArrayObject().bind();
    m_nItemsPerBatch = m_pCurrentShader->maxBatchedItems();

    for ( int batch = 0; batch < m_BatchTable.count(); ++batch )
    {
        const UniformBatchTable::UniformBatchOffsets& batchOffsets = m_BatchTable.at(batch);

        // The draw mode and line widths will be the same for all items
        // in the batch, so we can just sample them from the first item.
        m_nDrawMode = m_OffsetTable[batchOffsets.firstGeometryItem].drawMode;
        m_flLineWidth = m_OffsetTable[batchOffsets.firstGeometryItem].lineWidth;

        try
        {
            draw_x(batch);
        }
        catch (const InternalException&)
        {
            break;
        }
    }

    releaseBuffers();

    m_pCurrentShader->release();
    m_OpenGLBuffers.vertexArrayObject().release();
}

void GeometryRenderer::draw_x(int batch)
{
    const UniformBatchTable::UniformBatchOffsets& batchOffsets = m_BatchTable.at(batch);
    quint32 beginOffsetInts = m_OffsetTable.at(batchOffsets.firstGeometryItem).indexOffsetInts;

    const GeometryOffsetTable::ObjectOffsets endOffsets = m_OffsetTable.at(batchOffsets.lastGeometryItem);
    quint32 endOffsetInts = endOffsets.indexOffsetInts + endOffsets.indexCountInts;

    quint32 indicesCount = endOffsetInts - beginOffsetInts;

    bindBuffers_x(batch);

    GL_CURRENT_F;

    GLTRY(f->glLineWidth(m_flLineWidth));
    GLTRY(f->glDrawElements(m_nDrawMode, indicesCount, GL_UNSIGNED_INT, reinterpret_cast<GLvoid*>(beginOffsetInts * sizeof(quint32))));
}

void GeometryRenderer::bindBuffers_x(int batch)
{
    bool vBindSuccess = false;
    GLTRY(vBindSuccess = m_OpenGLBuffers.vertexBuffer().bind());
    if ( !vBindSuccess )
    {
        throw InternalException("Unable to bind vertex buffer.");
    }

    bool iBindSuccess = false;
    GLTRY(iBindSuccess = m_OpenGLBuffers.indexBuffer().bind());
    if ( !iBindSuccess )
    {
        throw InternalException("Unable to bind index buffer.");
    }

    const UniformBatchTable::UniformBatchOffsets& batchOffsets = m_BatchTable.at(batch);

    if ( m_pCurrentShader->hasLocalUniformBlockBinding() )
    {
        bool uBindSuccess = false;
        GLTRY(uBindSuccess = m_OpenGLBuffers.uniformBuffer().bindRange(PrivateShaderDefs::LocalUniformBlockBindingPoint,
                                                                       batchOffsets.batchOffsetBytes,
                                                                       batchOffsets.batchSizeBytes));
        if ( !uBindSuccess )
        {
            throw InternalException("Unable to bind uniform buffer.");
        }
    }
}

void GeometryRenderer::releaseBuffers()
{
    GLTRY(m_OpenGLBuffers.vertexBuffer().release());
    GLTRY(m_OpenGLBuffers.indexBuffer().release());
    GLTRY(m_OpenGLBuffers.uniformBuffer().release());
}
