#include "matrixbatchitem.h"

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
}
