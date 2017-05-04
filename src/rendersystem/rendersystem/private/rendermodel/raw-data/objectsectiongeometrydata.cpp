#include "objectsectiongeometrydata.h"

namespace
{
    template<typename T>
    inline quint32 totalVectorBytes(const QVector<T>& vec)
    {
        return vec.count() * sizeof(T);
    }
}

ObjectSectionGeometryData::ObjectSectionGeometryData()
    : m_bDirty(true),
      m_matModelToWorld(),
      m_Positions(),
      m_Normals(),
      m_Colors(),
      m_TextureCoordinates(),
      m_Indices()
{

}

bool ObjectSectionGeometryData::isDirty() const
{
    return m_bDirty;
}

void ObjectSectionGeometryData::markAsCleaned()
{
    m_bDirty = false;
}

void ObjectSectionGeometryData::markAsDirty()
{
    m_bDirty = true;
}

const QMatrix4x4& ObjectSectionGeometryData::modelToWorldMatrix() const
{
    return m_matModelToWorld;
}

void ObjectSectionGeometryData::setModelToWorldMatrix(const QMatrix4x4 &matrix)
{
    m_matModelToWorld = matrix;
    m_bDirty = true;
}

const QVector<float>& ObjectSectionGeometryData::positions() const
{
    return m_Positions;
}

void ObjectSectionGeometryData::setPositions(const QVector<float>& vec)
{
    m_Positions = vec;
    m_bDirty = true;
}

const QVector<float>& ObjectSectionGeometryData::normals() const
{
    return m_Normals;
}

void ObjectSectionGeometryData::setNormals(const QVector<float>& vec)
{
    m_Normals = vec;
    m_bDirty = true;
}

const QVector<float>& ObjectSectionGeometryData::colors() const
{
    return m_Colors;
}

void ObjectSectionGeometryData::setColors(const QVector<float>& vec)
{
    m_Colors = vec;
    m_bDirty = true;
}
const QVector<float>& ObjectSectionGeometryData::textureCoordinates() const
{
    return m_TextureCoordinates;
}

void ObjectSectionGeometryData::setTextureCoordinates(const QVector<float>& vec)
{
    m_TextureCoordinates = vec;
    m_bDirty = true;
}

const QVector<quint32>& ObjectSectionGeometryData::indices() const
{
    return m_Indices;
}

void ObjectSectionGeometryData::setIndices(const QVector<quint32> &vec)
{
    m_Indices = vec;
    m_bDirty = true;
}

quint32 ObjectSectionGeometryData::computeTotalVertexBytes() const
{
    return totalVectorBytes(m_Positions) +
            totalVectorBytes(m_Normals) +
            totalVectorBytes(m_Colors) +
            totalVectorBytes(m_TextureCoordinates);
}

quint32 ObjectSectionGeometryData::computeTotalIndexBytes() const
{
    return totalVectorBytes(m_Indices);
}
