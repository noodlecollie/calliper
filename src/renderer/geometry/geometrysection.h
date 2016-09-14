#ifndef GEOMETRYSECTION_H
#define GEOMETRYSECTION_H

#include "renderer_global.h"
#include <QVector2D>
#include <QVector3D>
#include <QVector4D>
#include <QColor>
#include "shaders/shaderdefs.h"
#include <QList>
#include <QVector>
#include "geometry/vertex3d.h"

namespace NS_RENDERER
{
    class RENDERERSHARED_EXPORT GeometrySection
    {
    public:
        enum AttributeType
        {
            PositionAttribute = 0,
            NormalAttribute,
            ColorAttribute,
            TextureCoordinateAttribute,

            AttributeTypeCount
        };

        GeometrySection();

        // It's assumed that the number of components for each attribute of
        // successive vertices will be the same (eg. 3 floats each time for position).
        // Don't start adding vertices with different numbers of components,
        // that's just asking for trouble.

        void add(AttributeType att, const float* data, int count);

        void addPosition(const QVector3D &pos);
        void addPosition(const QVector4D &pos);
        void addPositions(const float* data, int count, int components);
        void addNormal(const QVector3D &vec);
        void addColor(const QColor &col);
        void addTextureCoordinate(const QVector2D &coord);
        void addIndex(quint32 idx);
        void addIndexTriangle(quint32 i0, quint32 i1, quint32 i2);
        void addVertex(const Vertex3D &vertex);
        void addVertex(const Vertex3DValueRef &vertex);

        int floatCount(AttributeType att) const;
        int indexCount() const;
        int positionCount() const;    // How many times addPosition() has been called

        const float* vertexConstData(AttributeType type) const;
        const quint32* indexConstData() const;

        const QVector<float> vertexConstVector(AttributeType type) const;
        const QVector<quint32> indexConstVector() const;

        bool hasNormals() const;
        bool hasColors() const;
        bool hasTextureCoordinates() const;

    private:
        void init();

        QList<QVector<float> >  m_Attributes;
        QVector<quint32>        m_Indices;
        int                     m_iPositionCount;
    };

    typedef QList<GeometrySection> GeometrySectionList;
}

#endif // GEOMETRYSECTION_H
