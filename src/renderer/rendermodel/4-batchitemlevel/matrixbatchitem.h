#ifndef MATRIXBATCHITEM_H
#define MATRIXBATCHITEM_H

#include "renderer_global.h"
#include <QtGlobal>
#include <QVector>

namespace Renderer
{
    class MatrixBatchItemMetadata
    {
    public:
        MatrixBatchItemMetadata();
        void reset();

        int m_iPositionBytes;
        int m_iNormalBytes;
        int m_iColorBytes;
        int m_iTextureCoordinateBytes;
        int m_iIndexBytes;

        int totalVertexBytes() const;

        inline MatrixBatchItemMetadata& operator +=(const MatrixBatchItemMetadata &other)
        {
            m_iPositionBytes += other.m_iPositionBytes;
            m_iNormalBytes += other.m_iNormalBytes;
            m_iColorBytes += other.m_iColorBytes;
            m_iTextureCoordinateBytes += other.m_iTextureCoordinateBytes;
            m_iIndexBytes += other.m_iIndexBytes;

            return *this;
        }
    };

    // This is just a collection of data.
    // It is up to the managing classes to fill it as they see fit.
    class MatrixBatchItem
    {
    public:
        MatrixBatchItem();

        QVector<float>      m_Positions;
        QVector<float>      m_Normals;
        QVector<float>      m_Colors;
        QVector<float>      m_TextureCoordinates;
        QVector<quint32>    m_Indices;

        MatrixBatchItemMetadata buildMetadata() const;
        void clear();

        void printDebugInfo() const;
    };
}

#endif // MATRIXBATCHITEM_H
