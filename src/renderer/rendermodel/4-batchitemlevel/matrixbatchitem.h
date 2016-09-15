#ifndef MATRIXBATCHITEM_H
#define MATRIXBATCHITEM_H

#include "renderer_global.h"
#include <QtGlobal>
#include <QVector>

namespace NS_RENDERER
{
    // This is just a collection of data.
    // It is up to the managing classes to fill it as they see fit.
    struct MatrixBatchItem
    {
        MatrixBatchItem();

        QVector<float>      m_Positions;
        QVector<float>      m_Normals;
        QVector<float>      m_Colors;
        QVector<float>      m_TextureCoordinates;
        QVector<quint32>    m_Indices;
    };
}

#endif // MATRIXBATCHITEM_H
