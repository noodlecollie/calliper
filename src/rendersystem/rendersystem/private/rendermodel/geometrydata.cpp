#include "geometrydata.h"

namespace
{
    template<typename T>
    inline quint32 totalVectorBytes(const QVector<T>& vec)
    {
        return vec.count() * sizeof(T);
    }

    template<typename T>
    inline quint32 totalVectorFloatBytes(const QVector<T>& vec, int components)
    {
        return vec.count() * components * sizeof(float);
    }
}

GeometryData::GeometryData(quint32 objectId, quint8 sectionId)
    : m_bMatrixDirty(true),
      m_bVerticesDirty(true),
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

bool GeometryData::anyDirty() const
{
    return matrixDirty() || verticesDirty();
}

void GeometryData::clearAllDirtyFlags()
{
    setMatrixDirty(false);
    setVerticesDirty(false);
}

bool GeometryData::matrixDirty() const
{
    return m_bMatrixDirty;
}

void GeometryData::setMatrixDirty(bool dirty)
{
    m_bMatrixDirty = dirty;
}

bool GeometryData::verticesDirty() const
{
    return m_bVerticesDirty;
}

void GeometryData::setVerticesDirty(bool dirty)
{
    m_bVerticesDirty = dirty;
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
    setMatrixDirty(true);
}

const QVector<QVector4D>& GeometryData::positions() const
{
    return m_Positions;
}

void GeometryData::setPositions(const QVector<QVector4D>& vec)
{
    m_Positions = vec;
    setVerticesDirty(true);
}

QVector<QVector4D>& GeometryData::positionsRef()
{
    setVerticesDirty(true);
    return m_Positions;
}

const QVector<QVector4D>& GeometryData::normals() const
{
    return m_Normals;
}

void GeometryData::setNormals(const QVector<QVector4D>& vec)
{
    m_Normals = vec;
    setVerticesDirty(true);
}

QVector<QVector4D>& GeometryData::normalsRef()
{
    setVerticesDirty(true);
    return m_Normals;
}

const QVector<QVector4D>& GeometryData::colors() const
{
    return m_Colors;
}

void GeometryData::setColors(const QVector<QVector4D>& vec)
{
    m_Colors = vec;
    setVerticesDirty(true);
}

QVector<QVector4D>& GeometryData::colorsRef()
{
    setVerticesDirty(true);
    return m_Colors;
}

const QVector<QVector4D>& GeometryData::textureCoordinates() const
{
    return m_TextureCoordinates;
}

void GeometryData::setTextureCoordinates(const QVector<QVector4D>& vec)
{
    m_TextureCoordinates = vec;
    setVerticesDirty(true);
}

QVector<QVector4D>& GeometryData::textureCoordinatesRef()
{
    setVerticesDirty(true);
    return m_TextureCoordinates;
}

const QVector<quint32>& GeometryData::indices() const
{
    return m_Indices;
}

void GeometryData::setIndices(const QVector<quint32> &vec)
{
    m_Indices = vec;
    setVerticesDirty(true);
}

QVector<quint32>& GeometryData::indicesRef()
{
    setVerticesDirty(true);
    return m_Indices;
}

quint32 GeometryData::computeTotalVertexBytes(const VertexFormat &format) const
{
    return totalVectorFloatBytes(m_Positions, format.positionComponents()) +
            totalVectorFloatBytes(m_Normals, format.normalComponents()) +
            totalVectorFloatBytes(m_Colors, format.colorComponents()) +
            totalVectorFloatBytes(m_TextureCoordinates, format.textureCoordinateComponents());
}

quint32 GeometryData::computeTotalIndexBytes() const
{
    return totalVectorBytes(m_Indices);
}
