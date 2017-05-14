#include "geometrydata.h"

namespace
{
    template<typename T>
    inline quint32 totalVectorBytes(const QVector<T>& vec)
    {
        return vec.count() * sizeof(T);
    }

    template<>
    inline quint32 totalVectorBytes<QVector4D>(const QVector<QVector4D>& vec)
    {
        return vec.count() * 4 * sizeof(float);
    }
}

GeometryData::GeometryData(quint32 objectId, quint8 sectionId)
    : m_bDirty(true),
      m_nObjectId(objectId),
      m_nSectionId(sectionId),
      m_matModelToWorld(),
      m_Positions(),
      m_Normals(),
      m_Colors(),
      m_TextureCoordinates(),
      m_Indices()
{

}

bool GeometryData::dirty() const
{
    return m_bDirty;
}

void GeometryData::setDirty(bool dirty)
{
    m_bDirty = dirty;
}

quint32 GeometryData::objectId() const
{
    return m_nObjectId;
}

quint8 GeometryData::sectionId() const
{
    return m_nSectionId;
}

const QMatrix4x4& GeometryData::modelToWorldMatrix() const
{
    return m_matModelToWorld;
}

void GeometryData::setModelToWorldMatrix(const QMatrix4x4 &matrix)
{
    m_matModelToWorld = matrix;
    setDirty(true);
}

const QVector<QVector4D>& GeometryData::positions() const
{
    return m_Positions;
}

void GeometryData::setPositions(const QVector<QVector4D>& vec)
{
    m_Positions = vec;
    setDirty(true);
}

QVector<QVector4D>& GeometryData::positionsRef()
{
    setDirty(true);
    return m_Positions;
}

const QVector<QVector4D>& GeometryData::normals() const
{
    return m_Normals;
}

void GeometryData::setNormals(const QVector<QVector4D>& vec)
{
    m_Normals = vec;
    setDirty(true);
}

QVector<QVector4D>& GeometryData::normalsRef()
{
    setDirty(true);
    return m_Normals;
}

const QVector<QVector4D>& GeometryData::colors() const
{
    return m_Colors;
}

void GeometryData::setColors(const QVector<QVector4D>& vec)
{
    m_Colors = vec;
    setDirty(true);
}

QVector<QVector4D>& GeometryData::colorsRef()
{
    setDirty(true);
    return m_Colors;
}

const QVector<QVector4D>& GeometryData::textureCoordinates() const
{
    return m_TextureCoordinates;
}

void GeometryData::setTextureCoordinates(const QVector<QVector4D>& vec)
{
    m_TextureCoordinates = vec;
    setDirty(true);
}

QVector<QVector4D>& GeometryData::textureCoordinatesRef()
{
    setDirty(true);
    return m_TextureCoordinates;
}

const QVector<quint32>& GeometryData::indices() const
{
    return m_Indices;
}

void GeometryData::setIndices(const QVector<quint32> &vec)
{
    m_Indices = vec;
    setDirty(true);
}

QVector<quint32>& GeometryData::indicesRef()
{
    setDirty(true);
    return m_Indices;
}

quint32 GeometryData::computeTotalVertexBytes() const
{
    return totalVectorBytes(m_Positions) +
            totalVectorBytes(m_Normals) +
            totalVectorBytes(m_Colors) +
            totalVectorBytes(m_TextureCoordinates);
}

quint32 GeometryData::computeTotalIndexBytes() const
{
    return totalVectorBytes(m_Indices);
}
