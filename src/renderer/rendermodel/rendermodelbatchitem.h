#ifndef RENDERMODELBATCHITEM_H
#define RENDERMODELBATCHITEM_H

#include "renderer_global.h"
#include <QtGlobal>
#include <QVector>

namespace NS_RENDERER
{
    struct RenderModelBatchItem
    {
    public:
        RenderModelBatchItem();

        QVector<float>      m_Positions;
        QVector<float>      m_Normals;
        QVector<float>      m_Colors;
        QVector<float>      m_TextureCoordinates;
        QVector<quint32>    m_Indices;
        quint32             m_iObjectId;
    };
}

#endif // RENDERMODELBATCHITEM_H
