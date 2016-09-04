#include "geometrybuilder.h"
#include <QVector4D>

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
    class GeometryBuilder::Section
    {
    public:
        QVector<float>      m_Positions;
        QVector<float>      m_Normals;
        QVector<float>      m_Colors;
        QVector<float>      m_TextureCoordinates;
        QVector<quint32>    m_Indices;
        int                 m_iVerticesAdded;

        Section()
            : m_iVerticesAdded(0)
        {
        }

        bool attributeCountsEqual() const
        {
            int posCount = m_Positions.count();
            int nrmCount = m_Normals.count();
            int colCount = m_Colors.count();
            int txcCount = m_TextureCoordinates.count();

            return posCount == nrmCount &&
                    nrmCount == colCount &&
                    colCount == txcCount;
        }
    };

    GeometryBuilder::GeometryBuilder()
        : m_bUseVec4Positions(false)
    {
        createNewSection();
    }

    GeometryBuilder::~GeometryBuilder()
    {
        qDeleteAll(m_Sections);
    }

    void GeometryBuilder::addVertex(const Vertex3D &vertex)
    {
        addPosition(vertex.position());
        addNormal(vertex.normal());
        addColor(vertex.color());
        addTextureCoordinate(vertex.textureCoordinate());
    }

    void GeometryBuilder::addVertex(const Vertex3DValueRef &vertex)
    {
        addPosition(vertex.position());
        addNormal(vertex.normal());
        addColor(vertex.color());
        addTextureCoordinate(vertex.textureCoordinate());
    }

    GeometryBuilder::Section* GeometryBuilder::currentSection() const
    {
       return m_Sections.last();
    }

    void GeometryBuilder::addPosition(const QVector3D &pos)
    {
        if ( m_bUseVec4Positions )
        {
            append(currentSection()->m_Positions, QVector4D(pos, 1));
        }
        else
        {
            append(currentSection()->m_Positions, pos);
        }

        currentSection()->m_iVerticesAdded++;
    }

    void GeometryBuilder::addNormal(const QVector3D &nrm)
    {
        append(currentSection()->m_Normals, nrm);
    }

    void GeometryBuilder::addColor(const QColor &col)
    {
        append(currentSection()->m_Colors, col);
    }

    void GeometryBuilder::addTextureCoordinate(const QVector2D &coord)
    {
        append(currentSection()->m_TextureCoordinates, coord);
    }

    void GeometryBuilder::createNewSection()
    {
        m_Sections.append(new Section());
    }

    int GeometryBuilder::sectionCount() const
    {
        return m_Sections.count();
    }

    void GeometryBuilder::addIndex(quint32 index)
    {
        currentSection()->m_Indices.append(index);
    }

    void GeometryBuilder::addIndexTriangle(quint32 i0, quint32 i1, quint32 i2)
    {
        QVector<quint32> &indices = currentSection()->m_Indices;

        int count = indices.count();
        indices.resize(count + 3);

        quint32* data = indices.data();
        data[count] = i0;
        data[count+1] = i1;
        data[count+2] = i2;
    }

    int GeometryBuilder::positionComponentCount(int section) const
    {
        return m_Sections.at(section)->m_Positions.count();
    }

    int GeometryBuilder::normalComponentCount(int section) const
    {
        return m_Sections.at(section)->m_Normals.count();
    }

    int GeometryBuilder::colorComponentCount(int section) const
    {
        return m_Sections.at(section)->m_Colors.count();
    }
    int GeometryBuilder::textureCoordinateComponentCount(int section) const
    {
        return m_Sections.at(section)->m_TextureCoordinates.count();
    }

    int GeometryBuilder::totalAttributeComponentCount(int section) const
    {
        return positionComponentCount(section)
                + normalComponentCount(section)
                + colorComponentCount(section)
                + textureCoordinateComponentCount(section);
    }

    int GeometryBuilder::totalPositionComponentCount() const
    {
        int count = 0;
        foreach ( Section* section, m_Sections )
        {
            count += section->m_Positions.count();
        }

        return count;
    }

    int GeometryBuilder::totalNormalComponentCount() const
    {
        int count = 0;
        foreach ( Section* section, m_Sections )
        {
            count += section->m_Normals.count();
        }

        return count;
    }

    int GeometryBuilder::totalColorComponentCount() const
    {
        int count = 0;
        foreach ( Section* section, m_Sections )
        {
            count += section->m_Colors.count();
        }

        return count;
    }

    int GeometryBuilder::totalTextureCoordinateComponentCount() const
    {
        int count = 0;
        foreach ( Section* section, m_Sections )
        {
            count += section->m_TextureCoordinates.count();
        }

        return count;
    }

    int GeometryBuilder::totalAttributeComponentCountAcrossAllSections() const
    {
        int count = 0;
        for ( int i = 0; i < m_Sections.count(); i++ )
        {
            count += totalAttributeComponentCount(i);
        }

        return count;
    }

    bool GeometryBuilder::useVec4Positions() const
    {
        return m_bUseVec4Positions;
    }

    void GeometryBuilder::setUseVec4Positions(bool enabled)
    {
        m_bUseVec4Positions = enabled;
    }

    void GeometryBuilder::consolidate(QVector<float> &positions, QVector<float> normals,
                                      QVector<float> &colors, QVector<float> &textureCoordiates,
                                      QVector<quint32> &indices)
    {
        int verticesProcessed = 0;

        foreach ( Section* section, m_Sections )
        {
            positions.append(section->m_Positions);
            normals.append(section->m_Normals);
            colors.append(section->m_Colors);
            textureCoordiates.append(section->m_TextureCoordinates);

            // We need to renumber the indices from section-specific to global.
            int oldIndexCount = indices.count();
            indices.resize(oldIndexCount + section->m_Indices.count());
            quint32* indexData = indices.data() + oldIndexCount;

            for ( int i = 0; i < section->m_Indices.count(); i++ )
            {
                *indexData = verticesProcessed + section->m_Indices.at(i);
                indexData++;
            }

            verticesProcessed += section->m_iVerticesAdded;
        }
    }
}
