#include "geometryrenderer.h"

#include "renderutils.h"

#include "rendersystem/private/rendermodel/rendermodel-defs.h"

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
                                   GeometryOffsetTable &offsets,
                                   OpenGLBufferCollection &openGLBuffers)
    : m_Context(context),
      m_nMaterialId(materialId),
      m_OffsetTable(offsets),
      m_OpenGLBuffers(openGLBuffers),
      m_nItemsPerBatch(0),
      m_pCurrentShader(Q_NULLPTR),
      m_nDrawMode(RenderSystem::GeometrySection::DrawMode::DrawTriangles),
      m_flLineWidth(1.0f)
{
    m_pCurrentShader = RenderUtils::shaderFromMaterial(m_Context.renderMode(), m_nMaterialId);
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

    m_nItemsPerBatch = m_pCurrentShader->maxBatchedItems();

    int firstItem = 0;
    while ( firstItem < m_OffsetTable.count() )
    {
        int lastItem = getLastItemForNextDraw(firstItem);

        // The draw mode and line widths will be the same for all items
        // in the batch, so we can just sample them from the first item.
        m_nDrawMode = m_OffsetTable[firstItem].drawMode;
        m_flLineWidth = m_OffsetTable[firstItem].lineWidth;

        try
        {
            draw_x(firstItem, lastItem);
        }
        catch (const InternalException&)
        {
            break;
        }

        firstItem = lastItem + 1;
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

    GLTRY(f->glLineWidth(m_flLineWidth));
    f->glDrawElements(m_nDrawMode, indicesCount, GL_UNSIGNED_INT, reinterpret_cast<GLvoid*>(beginOffsetInts));
}
