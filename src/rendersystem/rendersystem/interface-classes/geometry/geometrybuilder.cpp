#include "geometrybuilder.h"

namespace RenderSystem
{
    GeometryBuilder::GeometryBuilder(PublicRenderModelDefs::ObjectId objectId,
                                     PublicStoreDefs::MaterialId materialId,
                                     const QMatrix4x4 &modelToWorldMatrix)
        : m_nObjectId(objectId),
          m_nMaterialId(materialId),
          m_matModelToWorld(modelToWorldMatrix),
          m_Sections()
    {
    }

    QSharedPointer<GeometrySection> GeometryBuilder::section(int index) const
    {
        if ( index < 0 || index > m_Sections.count() )
        {
            return QSharedPointer<GeometrySection>();
        }

        return m_Sections.at(index);
    }

    QSharedPointer<GeometrySection> GeometryBuilder::createNewSection(PublicStoreDefs::MaterialId materialId, const QMatrix4x4 &matrix)
    {
        if ( m_Sections.isEmpty() || !latestSection()->isEmpty() )
        {
            m_Sections.append(QSharedPointer<GeometrySection>::create(m_nObjectId, materialId, matrix));
        }

        return latestSection();
    }

    QSharedPointer<GeometrySection> GeometryBuilder::createNewSection()
    {
        return createNewSection(m_nMaterialId, m_matModelToWorld);
    }

    QSharedPointer<GeometrySection> GeometryBuilder::latestSection()
    {
        return section(sectionCount() - 1);
    }

    int GeometryBuilder::sectionCount() const
    {
        return m_Sections.count();
    }

    const QVector<QSharedPointer<GeometrySection> >& GeometryBuilder::sections() const
    {
        return m_Sections;
    }

    QMatrix4x4 GeometryBuilder::modelToWorldMatrix() const
    {
        return m_matModelToWorld;
    }

    void GeometryBuilder::setModelToWorldMatrix(const QMatrix4x4 &matrix)
    {
        m_matModelToWorld = matrix;
    }

    PublicStoreDefs::MaterialId GeometryBuilder::materialId() const
    {
        return m_nMaterialId;
    }

    void GeometryBuilder::setMaterialId(PublicStoreDefs::MaterialId id)
    {
        m_nMaterialId = id;
    }

    PublicRenderModelDefs::ObjectId GeometryBuilder::objectId() const
    {
        return m_nObjectId;
    }

    QSet<PublicStoreDefs::MaterialId> GeometryBuilder::referencedMaterials() const
    {
        QSet<PublicStoreDefs::MaterialId> materials;

        for ( int i = 0; i < sectionCount(); ++i )
        {
            materials.insert(section(i)->materialId());
        }

        return materials;
    }
}
