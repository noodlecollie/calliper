#include "vertexformat.h"

namespace NS_RENDERER
{
    VertexFormat::VertexFormat(int positions, int normals, int colors, int texCoords)
        : m_iPositionComponents(positions),
          m_iNormalComponents(normals),
          m_iColorComponents(colors),
          m_iTextureCoordinateComponents(texCoords)
    {

    }

    int VertexFormat::positionComponents() const
    {
        return m_iPositionComponents;
    }

    int VertexFormat::normalComponents() const
    {
        return m_iNormalComponents;
    }

    int VertexFormat::colorComponents() const
    {
        return m_iColorComponents;
    }

    int VertexFormat::textureCoordinateComponents() const
    {
        return m_iTextureCoordinateComponents;
    }

    int VertexFormat::totalVertexComponents() const
    {
        return positionComponents()
                + normalComponents()
                + colorComponents()
                + textureCoordinateComponents();
    }
}
