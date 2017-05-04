#include "geometrysection.h"

namespace
{
    inline bool attributeIndexValid(RenderSystem::GeometrySection::AttributeType attribute)
    {
        return attribute >= 0 && attribute < RenderSystem::GeometrySection::TOTAL_ATTRIBUTE_TYPES;
    }

    QVector<QVector4D> dummyVector;
}

namespace RenderSystem
{
    GeometrySection::GeometrySection()
        : m_nMaterialId(0),
          m_matModelToWorld(),
          m_AttributeVectors()
    {
    }

    GeometrySection::GeometrySection(PublicStoreDefs::MaterialId materialId, const QMatrix4x4 &modelToWorldMatrix)
        : m_nMaterialId(materialId),
          m_matModelToWorld(modelToWorldMatrix),
          m_AttributeVectors()
    {
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
}
