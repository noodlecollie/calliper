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

        quint32 offsetFloats() const;
        void setOffsetFloats(quint32 offset);

        quint32 countFloats() const;
        void setCountFloats(quint32 count);

        quint32 indexOffsetInts() const;
        void setIndexOffsetInts(quint32 indexOffset);

        quint32 indexCountInts() const;
        void setIndexCountInts(quint32 indexCount);

    private:
        quint32 m_iOffsetFloats;
        quint32 m_iCountFloats;
        quint32 m_iIndexOffsetInts;
        quint32 m_iIndexCountInts;
    };
}

#endif // RENDERMODELBATCHITEM_H
