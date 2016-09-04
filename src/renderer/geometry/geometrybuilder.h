#ifndef GEOMETRYBUILDER_H
#define GEOMETRYBUILDER_H

#include "renderer_global.h"
#include <QVector>
#include "vertex3d.h"

namespace NS_RENDERER
{
    class RENDERERSHARED_EXPORT GeometryBuilder
    {
    public:
        GeometryBuilder();
        ~GeometryBuilder();

        void createNewSection();
        int sectionCount() const;

        int positionComponentCount(int section) const;
        int normalComponentCount(int section) const;
        int colorComponentCount(int section) const;
        int textureCoordinateComponentCount(int section) const;
        int totalAttributeComponentCount(int section) const;

        int totalPositionComponentCount() const;
        int totalNormalComponentCount() const;
        int totalColorComponentCount() const;
        int totalTextureCoordinateComponentCount() const;
        int totalAttributeComponentCountAcrossAllSections() const;

        // If true, positions will be stored as QVector4Ds.
        // The final component will be set to 1.
        // This can be useful for batching, where the vertex
        // batch ID is kept in the last component of the
        // position vector. By default, this setting is false.
        bool useVec4Positions() const;
        void setUseVec4Positions(bool enabled);

        // The following functions apply to the current section.

        // It's assumed that the number of components for each attribute of
        // successive vertices will be the same (eg. 3 floats each time for position).
        // Don't start adding vertices with different numbers of components,
        // that's just asking for trouble.

        // -----------------------------------------------------
        void addVertex(const Vertex3D &vertex);
        void addVertex(const Vertex3DValueRef &vertex);

        void addPosition(const QVector3D &pos);
        void addNormal(const QVector3D &nrm);
        void addColor(const QColor &col);
        void addTextureCoordinate(const QVector2D &coord);

        // Indices are per section to make things simpler.
        void addIndex(quint32 index);
        void addIndexTriangle(quint32 i0, quint32 i1, quint32 i2);
        // -----------------------------------------------------

        // Export all data into buffers provided.
        void consolidate(QVector<float> &positions, QVector<float> normals,
                         QVector<float> &colors, QVector<float> &textureCoordiates,
                         QVector<quint32> &indices);

    private:
        class Section;

        Section* currentSection() const;

        QList<Section*>     m_Sections;
        bool    m_bUseVec4Positions;
    };
}

#endif // GEOMETRYBUILDER_H
