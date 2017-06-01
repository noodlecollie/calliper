#ifndef GEOMETRYBUILDER_H
#define GEOMETRYBUILDER_H

#include "rendersystem_global.h"

#include <QVector>
#include <QSharedPointer>
#include <QSet>

#include "rendersystem/interface-classes/store/publicstoredefs.h"
#include "rendersystem/interface-classes/rendermodel/publicrendermodeldefs.h"

#include "geometrysection.h"

namespace RenderSystem
{
    class RENDERSYSTEMSHARED_EXPORT GeometryBuilder
    {
    public:
        typedef QSharedPointer<GeometrySection> GeometrySectionPointer;

        GeometryBuilder(PublicRenderModelDefs::ObjectId objectId, PublicStoreDefs::MaterialId materialId, const QMatrix4x4& modelToWorldMatrix);

        GeometrySectionPointer section(int index) const;
        GeometrySectionPointer createNewSection(PublicStoreDefs::MaterialId materialId, const QMatrix4x4 &matrix);
        GeometrySectionPointer createNewSection();
        GeometrySectionPointer latestSection();
        const QVector<GeometrySectionPointer>& sections() const;
        int sectionCount() const;

        QMatrix4x4 modelToWorldMatrix() const;
        void setModelToWorldMatrix(const QMatrix4x4 &matrix);

        PublicStoreDefs::MaterialId materialId() const;
        void setMaterialId(PublicStoreDefs::MaterialId id);

        PublicRenderModelDefs::ObjectId objectId() const;

        QSet<PublicStoreDefs::MaterialId> referencedMaterials() const;

    private:
        PublicRenderModelDefs::ObjectId m_nObjectId;
        PublicStoreDefs::MaterialId m_nMaterialId;
        QMatrix4x4 m_matModelToWorld;
        QVector<GeometrySectionPointer> m_Sections;
    };
}

#endif // GEOMETRYBUILDER_H
