#ifndef RENDERMODELBATCHITEM_H
#define RENDERMODELBATCHITEM_H

#include "renderer_global.h"
#include <QtGlobal>
#include <QVector>

namespace NS_RENDERER
{
    class RenderModelBatchItem
    {
    public:
        RenderModelBatchItem();

        QVector<float>      m_Positions;
        QVector<float>      m_Normals;
        QVector<float>      m_Colors;
        QVector<float>      m_TextureCoordinates;
        QVector<quint32>    m_Indices;
    };
}

#endif // RENDERMODELBATCHITEM_H
