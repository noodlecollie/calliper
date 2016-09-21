#include "matrixbatchitem.h"
#include <QtDebug>

namespace NS_RENDERER
{
    MatrixBatchItemMetadata::MatrixBatchItemMetadata()
    {
        reset();
    }

    int MatrixBatchItemMetadata::totalVertexBytes() const
    {
        return m_iPositionBytes + m_iNormalBytes + m_iColorBytes + m_iTextureCoordinateBytes;
    }

    MatrixBatchItemMetadata MatrixBatchItem::buildMetadata() const
    {
        MatrixBatchItemMetadata meta;

        meta.m_iPositionBytes = m_Positions.count() * sizeof(float);
        meta.m_iNormalBytes = m_Normals.count() * sizeof(float);
        meta.m_iColorBytes = m_Colors.count() * sizeof(float);
        meta.m_iTextureCoordinateBytes = m_TextureCoordinates.count() * sizeof(float);
        meta.m_iIndexBytes = m_Indices.count() * sizeof(quint32);

        return meta;
    }

    void MatrixBatchItemMetadata::reset()
    {
        memset(this, 0, sizeof(MatrixBatchItemMetadata));
    }

    void MatrixBatchItem::clear()
    {
        m_Positions.clear();
        m_Normals.clear();
        m_Colors.clear();
        m_TextureCoordinates.clear();
        m_Indices.clear();
    }

    void MatrixBatchItem::printDebugInfo() const
    {
        qDebug() << "Positions:" << m_Positions.count()
                 << "Normals:" << m_Normals.count()
                 << "Colors:" << m_Colors.count()
                 << "Texture coordinates:" << m_TextureCoordinates.count();
    }
}
