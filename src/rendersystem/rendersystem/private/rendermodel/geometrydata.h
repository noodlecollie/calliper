#ifndef GEOMETRYDATA_H
#define GEOMETRYDATA_H

#include <QVector>
#include <QMatrix4x4>
#include <QVector4D>

class GeometryData
{
public:
    GeometryData(quint32 objectId, quint8 sectionId);

    bool dirty() const;
    void setDirty(bool dirty);

    quint32 objectId() const;
    quint8 sectionId() const;

    const QMatrix4x4& modelToWorldMatrix() const;
    void setModelToWorldMatrix(const QMatrix4x4& matrix);

    const QVector<QVector4D>& positions() const;
    void setPositions(const QVector<QVector4D>& vec);
    QVector<QVector4D>& positionsRef();

    const QVector<QVector4D>& normals() const;
    void setNormals(const QVector<QVector4D>& vec);
    QVector<QVector4D>& normalsRef();

    const QVector<QVector4D>& colors() const;
    void setColors(const QVector<QVector4D>& vec);
    QVector<QVector4D>& colorsRef();

    const QVector<QVector4D>& textureCoordinates() const;
    void setTextureCoordinates(const QVector<QVector4D>& vec);
    QVector<QVector4D>& textureCoordinatesRef();

    const QVector<quint32>& indices() const;
    void setIndices(const QVector<quint32>& vec);
    QVector<quint32>& indicesRef();

    quint32 computeTotalVertexBytes() const;
    quint32 computeTotalIndexBytes() const;

private:
    bool m_bDirty;

    quint32 m_nObjectId;
    quint8 m_nSectionId;
    QMatrix4x4 m_matModelToWorld;

    QVector<QVector4D> m_Positions;
    QVector<QVector4D> m_Normals;
    QVector<QVector4D> m_Colors;
    QVector<QVector4D> m_TextureCoordinates;
    QVector<quint32> m_Indices;
};

#endif // GEOMETRYDATA_H
