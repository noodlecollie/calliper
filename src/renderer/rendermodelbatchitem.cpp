#include "rendermodelbatchitem.h"

namespace NS_RENDERER
{
    RenderModelBatchItem::RenderModelBatchItem(quint32 offsetFloats, quint32 countFloats,
                                               quint32 indexOffsetInts, quint32 indexCountInts)
        : m_iOffsetFloats(offsetFloats), m_iCountFloats(countFloats), m_iIndexOffsetInts(indexOffsetInts),
          m_iIndexCountInts(indexCountInts)
    {

    }

    quint32 RenderModelBatchItem::offsetFloats() const
    {
        return m_iOffsetFloats;
    }

    quint32 RenderModelBatchItem::countFloats() const
    {
        return m_iCountFloats;
    }

    void RenderModelBatchItem::setOffsetFloats(quint32 offset)
    {
        m_iOffsetFloats = offset;
    }

    void RenderModelBatchItem::setCountFloats(quint32 count)
    {
        m_iCountFloats = count;
    }

    quint32 RenderModelBatchItem::indexCountInts() const
    {
        return m_iIndexCountInts;
    }

    void RenderModelBatchItem::setIndexCountInts(quint32 indexCount)
    {
        m_iIndexCountInts = indexCount;
    }

    quint32 RenderModelBatchItem::indexOffsetInts() const
    {
        return m_iIndexOffsetInts;
    }

    void RenderModelBatchItem::setIndexOffsetInts(quint32 indexOffset)
    {
        m_iIndexOffsetInts = indexOffset;
    }
}
