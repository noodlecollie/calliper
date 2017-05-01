#ifndef GEOMETRYBUILDER_H
#define GEOMETRYBUILDER_H

#include "rendersystem_global.h"

#include <QVector>
#include <QSharedPointer>

#include "rendersystem/interface-classes/store-defs/publicstoredefs.h"

#include "geometrysection.h"

namespace RenderSystem
{
    class RENDERSYSTEMSHARED_EXPORT GeometryBuilder
    {
    public:
        GeometryBuilder(PublicStoreDefs::MaterialId materialId, const QMatrix4x4& modelToWorldMatrix);

        QSharedPointer<GeometrySection> section(int index) const;
        QSharedPointer<GeometrySection> createNewSection(PublicStoreDefs::MaterialId materialId, const QMatrix4x4 &matrix);
        QSharedPointer<GeometrySection> createNewSection();
        QSharedPointer<GeometrySection> latestSection();
        const QVector<QSharedPointer<GeometrySection> >& sections() const;
        int sectionCount() const;

        QMatrix4x4 modelToWorldMatrix() const;
        void setModelToWorldMatrix(const QMatrix4x4 &matrix);

        PublicStoreDefs::MaterialId materialId() const;
        void setMaterialId(PublicStoreDefs::MaterialId id);

    private:
        PublicStoreDefs::MaterialId m_nMaterialId;
        QMatrix4x4 m_matModelToWorld;
        QVector<QSharedPointer<GeometrySection> > m_Sections;
    };
}

#endif // GEOMETRYBUILDER_H
