#include "geometrysection.h"

namespace
{
    void append(QVector<float> &list, const QVector2D &vec)
    {
        int count = list.count();
        list.resize(count + 2);

        float* data = list.data() + count;
        data[0] = vec.x();
        data[1] = vec.y();
    }

    void append(QVector<float> &list, const QVector3D &vec)
    {
        int count = list.count();
        list.resize(count + 3);

        float* data = list.data() + count;
        data[0] = vec.x();
        data[1] = vec.y();
        data[2] = vec.z();
    }

    void append(QVector<float> &list, const QVector4D &vec)
    {
        int count = list.count();
        list.resize(count + 4);

        float* data = list.data() + count;
        data[0] = vec.x();
        data[1] = vec.y();
        data[2] = vec.z();
        data[3] = vec.w();
    }

    void append(QVector<float> &list, const QColor &col)
    {
        int count = list.count();
        list.resize(count + 4);

        float* data = list.data() + count;
        data[0] = col.redF();
        data[1] = col.greenF();
        data[2] = col.blueF();
        data[3] = col.alphaF();
    }

    void append(QVector<float> &list, const float *data, int count)
    {
        int listSize = list.count();
        list.resize(listSize + count);

        float* listData = list.data() + listSize;
        for ( int i = 0; i < count; i++ )
        {
            listData[i] = data[i];
        }
    }
}

namespace NS_RENDERER
{
    GeometrySection::GeometrySection(quint16 shaderId, quint32 textureId, const QMatrix4x4 modelToWorldMatrix)
        : m_iPositionCount(0), m_iDrawMode(GL_TRIANGLES), m_flDrawWidth(1),
          m_iShaderId(shaderId), m_iTextureId(textureId),
          m_matModelToWorld(modelToWorldMatrix)
    {
        init();
    }

    void GeometrySection::init()
    {
        for ( int i = 0; i < AttributeTypeCount; i++ )
        {
            m_Attributes.append(QVector<float>());
        }
    }

    void GeometrySection::add(AttributeType att, const float *data, int count)
    {
        append(m_Attributes[att], data, count);
    }

    void GeometrySection::addPosition(const QVector3D &pos)
    {
        append(m_Attributes[PositionAttribute], pos);
        m_iPositionCount++;
    }

    void GeometrySection::addPosition(const QVector4D &pos)
    {
        append(m_Attributes[PositionAttribute], pos);
        m_iPositionCount++;
    }

    void GeometrySection::addNormal(const QVector3D &vec)
    {
        append(m_Attributes[NormalAttribute], vec);
    }

    void GeometrySection::addColor(const QColor &col)
    {
        append(m_Attributes[ColorAttribute], col);
    }

    void GeometrySection::addTextureCoordinate(const QVector2D &coord)
    {
        append(m_Attributes[TextureCoordinateAttribute], coord);
    }

    int GeometrySection::floatCount(AttributeType att) const
    {
        return m_Attributes.at(att).count();
    }

    int GeometrySection::indexCount() const
    {
        return m_Indices.count();
    }

    void GeometrySection::addIndex(quint32 idx)
    {
        m_Indices.append(idx);
    }

    void GeometrySection::addIndexTriangle(quint32 i0, quint32 i1, quint32 i2)
    {
        m_Indices.reserve(m_Indices.count() + 3);
        m_Indices.append(i0);
        m_Indices.append(i1);
        m_Indices.append(i2);
    }

    void GeometrySection::addIndexLine(quint32 i0, quint32 i1)
    {
        m_Indices.reserve(m_Indices.count() + 2);
        m_Indices.append(i0);
        m_Indices.append(i1);
    }

    void GeometrySection::addVertex(const Vertex3D &vertex)
    {
        addPosition(vertex.position());
        addNormal(vertex.normal());
        addColor(vertex.color());
        addTextureCoordinate(vertex.textureCoordinate());
    }

    void GeometrySection::addVertex(const Vertex3DValueRef &vertex)
    {
        addPosition(vertex.position());
        addNormal(vertex.normal());
        addColor(vertex.color());
        addTextureCoordinate(vertex.textureCoordinate());
    }

    const float* GeometrySection::vertexConstData(AttributeType type) const
    {
        return m_Attributes.at(type).constData();
    }

    const quint32* GeometrySection::indexConstData() const
    {
        return m_Indices.constData();
    }

    const QVector<float> GeometrySection::vertexConstVector(AttributeType type) const
    {
        return m_Attributes.at(type);
    }

    const QVector<quint32> GeometrySection::indexConstVector() const
    {
        return m_Indices;
    }

    int GeometrySection::positionCount() const
    {
        return m_iPositionCount;
    }

    bool GeometrySection::hasNormals() const
    {
        return !m_Attributes[NormalAttribute].isEmpty();
    }

    bool GeometrySection::hasColors() const
    {
        return !m_Attributes[ColorAttribute].isEmpty();
    }

    bool GeometrySection::hasTextureCoordinates() const
    {
        return !m_Attributes[TextureCoordinateAttribute].isEmpty();
    }

    void GeometrySection::addPositions(const float *data, int count, int components)
    {
        add(PositionAttribute, data, count);
        m_iPositionCount += count/components;
    }

    int GeometrySection::consolidate(QVector<float> &positions, QVector<float> &normals, QVector<float> &colors,
                                      QVector<float> &textureCoordinates, QVector<quint32> &indices) const
    {
        positions.append(vertexConstVector(GeometrySection::PositionAttribute));
        normals.append(vertexConstVector(GeometrySection::NormalAttribute));
        colors.append(vertexConstVector(GeometrySection::ColorAttribute));
        textureCoordinates.append(vertexConstVector(GeometrySection::TextureCoordinateAttribute));

        // We need to renumber the indices from section-specific to global.
        int oldIndexCount = indices.count();
        indices.resize(oldIndexCount + indexCount());
        quint32* indexData = indices.data() + oldIndexCount;
        const quint32* indexSource = indexConstData();

        for ( int i = 0; i < indexCount(); i++ )
        {
            indexData[i] = oldIndexCount + indexSource[i];
        }

        return indexCount();
    }

    quint16 GeometrySection::shaderId() const
    {
        return m_iShaderId;
    }

    void GeometrySection::setShaderId(quint16 id)
    {
        m_iShaderId = id;
    }

    quint32 GeometrySection::textureId() const
    {
        return m_iTextureId;
    }

    void GeometrySection::setTextureId(quint32 id)
    {
        m_iTextureId = id;
    }

    GLenum GeometrySection::drawMode() const
    {
        return m_iDrawMode;
    }

    void GeometrySection::setDrawMode(GLenum mode)
    {
        m_iDrawMode = mode;
    }

    float GeometrySection::drawWidth() const
    {
        return m_flDrawWidth;
    }

    void GeometrySection::setDrawWidth(float width)
    {
        m_flDrawWidth = width;
    }

    const QMatrix4x4& GeometrySection::modelToWorldMatrix() const
    {
        return m_matModelToWorld;
    }

    void GeometrySection::setModelToWorldMatrix(const QMatrix4x4 &mat)
    {
        m_matModelToWorld = mat;
    }

    bool GeometrySection::isEmpty() const
    {
        return positionCount() < 1 && indexCount() < 1;
    }
}
