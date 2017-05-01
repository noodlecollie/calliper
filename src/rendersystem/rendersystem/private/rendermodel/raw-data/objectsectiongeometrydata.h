#ifndef OBJECTSECTIONGEOMETRYDATA_H
#define OBJECTSECTIONGEOMETRYDATA_H

#include <QVector>

struct ObjectSectionGeometryData
{
    QVector<float> m_Positions;
    QVector<float> m_Normals;
    QVector<float> m_Colors;
    QVector<float> m_TextureCoordinates;
    QVector<quint32> m_Indices;

    template<typename T>
    inline quint32 totalBytes(const QVector<T>& vec)
    {
        return vec.count() * sizeof(T);
    }

    inline quint32 totalBytes() const
    {
        return totalBytes(m_Positions) +
                totalBytes(m_Normals) +
                totalBytes(m_Colors) +
                totalBytes(m_TextureCoordinates) +
                totalBytes(m_Indices);

    }
};

#endif // OBJECTSECTIONGEOMETRYDATA_H
