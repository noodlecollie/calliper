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

        // It's assumed that the number of components for each attribute of
        // successive vertices will be the same (eg. 3 floats each time for position).
        // Don't start adding vertices with different numbers of components,
        // that's just asking for trouble.
        void addVertex(const Vertex3D &vertex);

    private:
        QVector<float>  m_Positions;
        QVector<float>  m_Normals;
        QVector<float>  m_Colors;
        QVector<float>  m_TextureCoordinates;
    };
}

#endif // GEOMETRYBUILDER_H
