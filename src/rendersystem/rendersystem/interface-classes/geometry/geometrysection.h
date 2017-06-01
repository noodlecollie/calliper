#ifndef GEOMETRYSECTION_H
#define GEOMETRYSECTION_H

#include "rendersystem_global.h"

#include <QOpenGLContext>
#include <QVector>
#include <QVector2D>
#include <QVector4D>
#include <QColor>
#include <QMatrix4x4>

#include "rendersystem/interface-classes/definitions/materialdefs.h"
#include "rendersystem/interface-classes/definitions/rendermodeldefs.h"

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
                        RenderModelDefs::ObjectId objectId,
                        MaterialDefs::MaterialId materialId,
                        const QMatrix4x4& modelToWorldMatrix);

        quint8 sectionId() const;
        RenderModelDefs::ObjectId objectId() const;

        MaterialDefs::MaterialId materialId() const;
        void setMaterialId(MaterialDefs::MaterialId id);

        GLenum drawMode() const;
        void setDrawMode(GLenum mode);

        float lineWidth() const;
        void setLineWidth(float width);

        QMatrix4x4 modelToWorldMatrix() const;
        void setModelToWorldMatrix(const QMatrix4x4& mat);

        // Data is held internally as QVector4Ds.
        // Elements not provided will be set to 0.
        void addAttribute(AttributeType attribute, const QVector2D& vec);
        void addAttribute(AttributeType attribute, const QVector3D& vec);
        void addAttribute(AttributeType attribute, const QVector4D& vec);
        void addAttribute(AttributeType attribute, const QColor& col);

        void clearAttribute(AttributeType attribute);
        void clearAllAttributes();

        template<typename T>
        void addAttributeVector(AttributeType attribute, const QVector<T>& vec);

        inline void addPosition(const QVector3D& pos);
        inline void addNormal(const QVector3D& normal);
        inline void addColor(const QColor& col);
        inline void addTextureCoordinate(const QVector2D& coord);

        int count(AttributeType attribute) const;
        bool allNonZeroAttributeCountsEqual() const;
        bool isEmpty() const;

        bool calculateIndices();
        void addIndex(quint32 index);
        void addIndexPair(quint32 index0, quint32 index1);
        void addIndexTriangle(quint32 index0, quint32 index1, quint32 index2);

        const QVector<QVector4D>& attributeVector(AttributeType attribute) const;
        const QVector<quint32>& indicesVector() const;

    private:
        static inline bool attributeIndexValid(RenderSystem::GeometrySection::AttributeType attribute)
        {
            return attribute >= 0 && attribute < RenderSystem::GeometrySection::TOTAL_ATTRIBUTE_TYPES;
        }

        bool calculateTriangleIndices();
        bool calculateLinearIndices(int minRequiredPositions);

        quint8 m_nSectionId;
        RenderModelDefs::ObjectId m_nObjectId;
        MaterialDefs::MaterialId m_nMaterialId;
        QMatrix4x4 m_matModelToWorld;
        GLenum m_nDrawMode;
        float m_flLineWidth;
        QVector<QVector4D> m_AttributeVectors[TOTAL_ATTRIBUTE_TYPES];
        QVector<quint32> m_Indices;
    };

    void GeometrySection::addPosition(const QVector3D& pos)
    {
        addAttribute(Position, pos);
    }

    void GeometrySection::addNormal(const QVector3D& normal)
    {
        addAttribute(Normal, normal);
    }

    void GeometrySection::addColor(const QColor& col)
    {
        addAttribute(Color, col);
    }

    void GeometrySection::addTextureCoordinate(const QVector2D& coord)
    {
        addAttribute(TextureCoordinate, coord);
    }

    template<typename T>
    void GeometrySection::addAttributeVector(AttributeType attribute, const QVector<T>& vec)
    {
        if ( !attributeIndexValid(attribute) )
        {
            return;
        }

        QVector<QVector4D>& attributeVec = m_AttributeVectors[attribute];
        const int baseSize = attributeVec.count();
        attributeVec.reserve(baseSize + vec.count());

        for ( typename QVector<T>::const_iterator itItem = vec.constBegin();
              itItem != vec.constEnd();
              ++itItem )
        {
            addAttribute(attribute, *itItem);
        }
    }
}

#endif // GEOMETRYSECTION_H
