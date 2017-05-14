#ifndef GEOMETRYSECTION_H
#define GEOMETRYSECTION_H

#include "rendersystem_global.h"

#include <QVector>
#include <QVector4D>
#include <QColor>
#include <QMatrix4x4>

#include "rendersystem/interface-classes/store-defs/publicstoredefs.h"
#include "rendersystem/interface-classes/rendermodel-defs/publicrendermodeldefs.h"

namespace RenderSystem
{
    class RENDERSYSTEMSHARED_EXPORT GeometrySection
    {
    public:
        enum AttributeType
        {
            Position = 0,
            Normal,
            Color,
            TextureCoordinate,

            TOTAL_ATTRIBUTE_TYPES
        };

        GeometrySection();
        GeometrySection(quint8 sectionId,
                        PublicRenderModelDefs::ObjectId objectId,
                        PublicStoreDefs::MaterialId materialId,
                        const QMatrix4x4& modelToWorldMatrix);

        quint8 sectionId() const;
        PublicRenderModelDefs::ObjectId objectId() const;

        PublicStoreDefs::MaterialId materialId() const;
        void setMaterialId(PublicStoreDefs::MaterialId id);

        QMatrix4x4 modelToWorldMatrix() const;
        void setModelToWorldMatrix(const QMatrix4x4& mat);

        // Data is held internally as QVector4Ds.
        // Elements not provided will be set to 0.
        void addAttribute(AttributeType attribute, const QVector2D& vec);
        void addAttribute(AttributeType attribute, const QVector3D& vec);
        void addAttribute(AttributeType attribute, const QVector4D& vec);
        void addAttribute(AttributeType attribute, const QColor& col);
        void clearAllAttributes();

        int count(AttributeType attribute) const;
        bool allNonZeroAttributeCountsEqual() const;
        bool isEmpty() const;

        const QVector<QVector4D>& attributeVector(AttributeType attribute) const;

    private:
        quint8 m_nSectionId;
        PublicRenderModelDefs::ObjectId m_nObjectId;
        PublicStoreDefs::MaterialId m_nMaterialId;
        QMatrix4x4 m_matModelToWorld;
        QVector<QVector4D> m_AttributeVectors[TOTAL_ATTRIBUTE_TYPES];
    };
}

#endif // GEOMETRYSECTION_H
