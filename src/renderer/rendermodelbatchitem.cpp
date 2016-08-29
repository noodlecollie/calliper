#include "rendermodelbatchitem.h"

namespace NS_RENDERER
{
    RenderModelBatchItem::RenderModelBatchItem(quint32 vertexOffset, quint32 vertexCount,
                                               quint32 indexOffset, quint32 indexCount)
        : m_iVertexOffset(vertexOffset), m_iVertexCount(vertexCount),
          m_iIndexOffset(indexOffset), m_iIndexCount(indexCount)
    {

    }

    quint32 RenderModelBatchItem::vertexOffset() const
    {
        return m_iVertexOffset;
    }

    quint32 RenderModelBatchItem::vertexCount() const
    {
        return m_iVertexCount;
    }

    void RenderModelBatchItem::setVertexOffset(quint32 offset)
    {
        m_iVertexOffset = offset;
    }

    void RenderModelBatchItem::setVertexCount(quint32 count)
    {
        m_iVertexCount = count;
    }

    quint32 RenderModelBatchItem::indexCount() const
    {
        return m_iIndexCount;
    }

    void RenderModelBatchItem::setIndexCount(quint32 indexCount)
    {
        m_iIndexCount = indexCount;
    }

    quint32 RenderModelBatchItem::indexOffset() const
    {
        return m_iIndexOffset;
    }

    void RenderModelBatchItem::setIndexOffset(quint32 indexOffset)
    {
        m_iIndexOffset = indexOffset;
    }
}
