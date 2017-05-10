#ifndef OBJECTSECTIONGEOMETRYDATA_H
#define OBJECTSECTIONGEOMETRYDATA_H

#include <QVector>
#include <QMatrix4x4>

class ObjectSectionGeometryData
{
public:
    ObjectSectionGeometryData(quint32 objectId, quint8 sectionId);

    bool isDirty() const;
    void markAsCleaned();
    void markAsDirty();

    quint32 objectId() const;
    quint8 sectionId() const;

    const QMatrix4x4& modelToWorldMatrix() const;
    void setModelToWorldMatrix(const QMatrix4x4& matrix);

    const QVector<float>& positions() const;
    void setPositions(const QVector<float>& vec);

    const QVector<float>& normals() const;
    void setNormals(const QVector<float>& vec);

    const QVector<float>& colors() const;
    void setColors(const QVector<float>& vec);

    const QVector<float>& textureCoordinates() const;
    void setTextureCoordinates(const QVector<float>& vec);

    const QVector<quint32>& indices() const;
    void setIndices(const QVector<quint32>& vec);

    quint32 computeTotalVertexBytes() const;
    quint32 computeTotalIndexBytes() const;

private:
    bool m_bDirty;

    quint32 m_nObjectId;
    quint8 m_nSectionId;
    QMatrix4x4 m_matModelToWorld;

    QVector<float> m_Positions;
    QVector<float> m_Normals;
    QVector<float> m_Colors;
    QVector<float> m_TextureCoordinates;
    QVector<quint32> m_Indices;
};

#endif // OBJECTSECTIONGEOMETRYDATA_H
