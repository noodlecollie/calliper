#include "geometrysection.h"

namespace
{
    QVector<QVector4D> dummyVector;
}

namespace RenderSystem
{
    GeometrySection::GeometrySection()
        : m_nSectionId(0),
          m_nObjectId(0),
          m_nMaterialId(PublicStoreDefs::INVALID_MATERIAL_ID),
          m_matModelToWorld(),
          m_nDrawMode(GL_TRIANGLES),
          m_AttributeVectors(),
          m_Indices()
    {
    }

    GeometrySection::GeometrySection(quint8 sectionId,
                                     PublicRenderModelDefs::ObjectId objectId,
                                     PublicStoreDefs::MaterialId materialId,
                                     const QMatrix4x4 &modelToWorldMatrix)
        : m_nSectionId(sectionId),
          m_nObjectId(objectId),
          m_nMaterialId(materialId),
          m_matModelToWorld(modelToWorldMatrix),
          m_nDrawMode(GL_TRIANGLES),
          m_AttributeVectors(),
          m_Indices()
    {
    }

    quint8 GeometrySection::sectionId() const
    {
        return m_nSectionId;
    }

    PublicRenderModelDefs::ObjectId GeometrySection::objectId() const
    {
        return m_nObjectId;
    }

    PublicStoreDefs::MaterialId GeometrySection::materialId() const
    {
        return m_nMaterialId;
    }

    void GeometrySection::setMaterialId(PublicStoreDefs::MaterialId id)
    {
        m_nMaterialId = id;
    }

    QMatrix4x4 GeometrySection::modelToWorldMatrix() const
    {
        return m_matModelToWorld;
    }

    void GeometrySection::setModelToWorldMatrix(const QMatrix4x4 &mat)
    {
        m_matModelToWorld = mat;
    }

    GLenum GeometrySection::drawMode() const
    {
        Q_ASSERT_X(false, Q_FUNC_INFO, "Implement this in renderer!");
        return m_nDrawMode;
    }

    void GeometrySection::setDrawMode(GLenum mode)
    {
        Q_ASSERT_X(false, Q_FUNC_INFO, "Implement this in renderer!");
        m_nDrawMode = mode;
    }

    void GeometrySection::addAttribute(AttributeType attribute, const QVector4D& vec)
    {
        if ( !attributeIndexValid(attribute) )
        {
            return;
        }

        m_AttributeVectors[attribute].append(vec);
    }

    void GeometrySection::addAttribute(AttributeType attribute, const QVector3D &vec)
    {
        addAttribute(attribute, QVector4D(vec, 0));
    }

    void GeometrySection::addAttribute(AttributeType attribute, const QVector2D &vec)
    {
        addAttribute(attribute, QVector4D(vec, 0, 0));
    }

    void GeometrySection::addAttribute(AttributeType attribute, const QColor &col)
    {
        addAttribute(attribute, QVector4D(col.redF(), col.greenF(), col.blueF(), col.alphaF()));
    }

    int GeometrySection::count(AttributeType attribute) const
    {
        if ( !attributeIndexValid(attribute) )
        {
            return 0;
        }

        return m_AttributeVectors[attribute].count();
    }

    bool GeometrySection::allNonZeroAttributeCountsEqual() const
    {
        int targetAttributeCount = 0;

        for ( int att = 0; att < TOTAL_ATTRIBUTE_TYPES; ++att )
        {
            int count = m_AttributeVectors[att].count();

            if ( count < 1 )
            {
                continue;
            }

            if ( targetAttributeCount < 1 )
            {
                targetAttributeCount = count;
                continue;
            }

            if ( count != targetAttributeCount )
            {
                return false;
            }
        }

        return true;
    }

    void GeometrySection::clearAllAttributes()
    {
        for ( int att = 0; att < TOTAL_ATTRIBUTE_TYPES; ++att )
        {
            m_AttributeVectors[att].clear();
        }
    }

    void GeometrySection::clearAttribute(AttributeType attribute)
    {
        if ( !attributeIndexValid(attribute) )
        {
            return;
        }

        m_AttributeVectors[attribute].clear();
    }

    bool GeometrySection::isEmpty() const
    {
        for ( int att = 0; att < TOTAL_ATTRIBUTE_TYPES; ++att )
        {
            if ( count(static_cast<AttributeType>(att)) > 0 )
            {
                return false;
            }
        }

        return true;
    }

    const QVector<QVector4D>& GeometrySection::attributeVector(AttributeType attribute) const
    {
        if ( !attributeIndexValid(attribute) )
        {
            return dummyVector;
        }

        return m_AttributeVectors[attribute];
    }

    const QVector<quint32>& GeometrySection::indicesVector() const
    {
        return m_Indices;
    }

    void GeometrySection::addIndex(quint32 index)
    {
        m_Indices.append(index);
    }

    void GeometrySection::addIndexPair(quint32 index0, quint32 index1)
    {
        m_Indices.resize(m_Indices.count() + 2);
        m_Indices[m_Indices.count() - 2] = index0;
        m_Indices[m_Indices.count() - 1] = index1;
    }

    void GeometrySection::addIndexTriangle(quint32 index0, quint32 index1, quint32 index2)
    {
        m_Indices.resize(m_Indices.count() + 3);
        m_Indices[m_Indices.count() - 3] = index0;
        m_Indices[m_Indices.count() - 2] = index1;
        m_Indices[m_Indices.count() - 1] = index2;
    }

    bool GeometrySection::calculateIndices()
    {
        m_Indices.clear();

        if ( !allNonZeroAttributeCountsEqual() )
        {
            return false;
        }

        switch ( m_nDrawMode )
        {
            case GL_TRIANGLES:
            {
                return calculateTriangleIndices();
            }

            case GL_LINES:
            case GL_LINE_LOOP:
            {
                return calculateLinearIndices(2);
            }

            case GL_TRIANGLE_FAN:
            {
                return calculateLinearIndices(3);
            }

            default:
            {
                return false;
            }
        }
    }

    bool GeometrySection::calculateTriangleIndices()
    {
        const int positionCount = count(Position);
        if ( positionCount < 3 )
        {
            return false;
        }

        // (positionCount - 2) triangles, with 3 indices per triangle.
        m_Indices.resize((positionCount - 2) * 3);

        int indexOffset = 0;
        for ( quint32 index = 2; index < static_cast<quint32>(positionCount); ++index )
        {
            m_Indices[indexOffset] = 0;
            m_Indices[indexOffset + 1] = index - 1;
            m_Indices[indexOffset + 2] = index;

            indexOffset += 3;
        }

        return true;
    }

    bool GeometrySection::calculateLinearIndices(int minRequiredPositions)
    {
        const int positionCount = count(Position);
        if ( positionCount < minRequiredPositions )
        {
            return false;
        }

        m_Indices.resize(positionCount);

        for ( quint32 index = 0; index < static_cast<quint32>(positionCount); ++index )
        {
            m_Indices[index] = index;
        }

        return true
    }
}
