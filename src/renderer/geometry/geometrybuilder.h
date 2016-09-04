#ifndef GEOMETRYBUILDER_H
#define GEOMETRYBUILDER_H

#include "renderer_global.h"
#include "geometry/geometrysection.h"

namespace NS_RENDERER
{
    class RENDERERSHARED_EXPORT GeometryBuilder
    {
    public:
        GeometryBuilder();
        ~GeometryBuilder();

        int sectionCount() const;
        GeometrySection& section(int index);
        const GeometrySection& section(int index) const;
        GeometrySection& createNewSection();
        GeometrySection& currentSection();

        // Export all data into buffers provided.
        void consolidate(QVector<float> &positions, QVector<float> normals,
                         QVector<float> &colors, QVector<float> &textureCoordiates,
                         QVector<quint32> &indices) const;

    private:
        QList<GeometrySection>  m_Sections;
    };
}

#endif // GEOMETRYBUILDER_H
