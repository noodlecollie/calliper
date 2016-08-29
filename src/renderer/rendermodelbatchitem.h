#ifndef RENDERMODELBATCHITEM_H
#define RENDERMODELBATCHITEM_H

#include "renderer_global.h"
#include <QtGlobal>

namespace NS_RENDERER
{
    class RenderModelBatchItem
    {
    public:
        RenderModelBatchItem(quint32 offsetFloats, quint32 countFloats,
                             quint32 indexOffsetInts, quint32 indexCountInts);

        // This is in vertices, so varies in bytes
        // depending on how many components the attribute has.
        quint32 vertexOffset() const;
        void setVertexOffset(quint32 offset);

        quint32 vertexCount() const;
        void setVertexCount(quint32 count);

        // This is in indices, so should be multiplied by
        // sizeof(quint32) to get the offset in bytes.
        quint32 indexOffset() const;
        void setIndexOffset(quint32 indexOffset);

        quint32 indexCount() const;
        void setIndexCount(quint32 indexCount);

    private:
        quint32 m_iVertexOffset;
        quint32 m_iVertexCount;
        quint32 m_iIndexOffset;
        quint32 m_iIndexCount;
    };
}

#endif // RENDERMODELBATCHITEM_H
