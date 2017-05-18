#include "vertexformat.h"
#include <QtDebug>

VertexFormat::VertexFormat(int positions, int normals, int colors, int texCoords)
    : m_iPositionComponents(positions),
      m_iNormalComponents(normals),
      m_iColorComponents(colors),
      m_iTextureCoordinateComponents(texCoords)
{
    Q_ASSERT_X(positions >= 0 && positions <= 4, Q_FUNC_INFO, "Positions value must be between 0 and 4");
    Q_ASSERT_X(normals >= 0 && normals <= 4, Q_FUNC_INFO, "Normals value must be between 0 and 4");
    Q_ASSERT_X(colors >= 0 && colors <= 4, Q_FUNC_INFO, "Colours value must be between 0 and 4");
    Q_ASSERT_X(texCoords >= 0 && texCoords <= 4, Q_FUNC_INFO, "Texture coordinates value must be between 0 and 4");
}

VertexFormat::VertexFormat()
    : VertexFormat(0,0,0,0)
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

bool VertexFormat::operator ==(const VertexFormat& other) const
{
    return m_iPositionComponents == other.m_iPositionComponents &&
            m_iNormalComponents == other.m_iNormalComponents &&
            m_iColorComponents == other.m_iColorComponents &&
            m_iTextureCoordinateComponents == other.m_iTextureCoordinateComponents;
}

bool VertexFormat::operator !=(const VertexFormat& other) const
{
    return !(*this == other);
}
