#include "geometrybuilder.h"

namespace RenderSystem
{
    GeometryBuilder::GeometryBuilder(PublicRenderModelDefs::ObjectId objectId,
                                     MaterialDefs::MaterialId materialId,
                                     const QMatrix4x4 &modelToWorldMatrix)
        : m_nObjectId(objectId),
          m_nMaterialId(materialId),
          m_matModelToWorld(modelToWorldMatrix),
          m_Sections()
    {
    }

    GeometryBuilder::GeometrySectionPointer GeometryBuilder::section(int index) const
    {
        if ( index < 0 || index > m_Sections.count() )
        {
            return GeometrySectionPointer();
        }

        return m_Sections.at(index);
    }

    GeometryBuilder::GeometrySectionPointer GeometryBuilder::createNewSection(MaterialDefs::MaterialId materialId, const QMatrix4x4 &matrix)
    {
        if ( m_Sections.isEmpty() || !latestSection()->isEmpty() )
        {
            m_Sections.append(GeometrySectionPointer::create(m_Sections.count(), m_nObjectId, materialId, matrix));
        }

        return latestSection();
    }

    GeometryBuilder::GeometrySectionPointer GeometryBuilder::createNewSection()
    {
        return createNewSection(m_nMaterialId, m_matModelToWorld);
    }

    GeometryBuilder::GeometrySectionPointer GeometryBuilder::latestSection()
    {
        return section(sectionCount() - 1);
    }

    int GeometryBuilder::sectionCount() const
    {
        return m_Sections.count();
    }

    const QVector<GeometryBuilder::GeometrySectionPointer>& GeometryBuilder::sections() const
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

    MaterialDefs::MaterialId GeometryBuilder::materialId() const
    {
        return m_nMaterialId;
    }

    void GeometryBuilder::setMaterialId(MaterialDefs::MaterialId id)
    {
        m_nMaterialId = id;
    }

    PublicRenderModelDefs::ObjectId GeometryBuilder::objectId() const
    {
        return m_nObjectId;
    }

    QSet<MaterialDefs::MaterialId> GeometryBuilder::referencedMaterials() const
    {
        QSet<MaterialDefs::MaterialId> materials;

        for ( int i = 0; i < sectionCount(); ++i )
        {
            materials.insert(section(i)->materialId());
        }

        return materials;
    }
}
