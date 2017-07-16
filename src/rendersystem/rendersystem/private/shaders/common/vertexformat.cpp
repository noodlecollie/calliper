#include "vertexformat.h"
#include <QtDebug>

// Thanks Microsoft.
#ifdef Q_OS_WIN
#pragma warning (disable:4351)
#endif

VertexFormat::VertexFormat(int positions, int normals, int colors, int texCoords)
    : m_nComponents{}
{
    Q_ASSERT_X(positions >= 0 && positions <= 4, Q_FUNC_INFO, "Positions value must be between 0 and 4");
    Q_ASSERT_X(normals >= 0 && normals <= 4, Q_FUNC_INFO, "Normals value must be between 0 and 4");
    Q_ASSERT_X(colors >= 0 && colors <= 4, Q_FUNC_INFO, "Colours value must be between 0 and 4");
    Q_ASSERT_X(texCoords >= 0 && texCoords <= 4, Q_FUNC_INFO, "Texture coordinates value must be between 0 and 4");

    m_nComponents[PrivateShaderDefs::PositionAttribute] = positions;
    m_nComponents[PrivateShaderDefs::NormalAttribute] = normals;
    m_nComponents[PrivateShaderDefs::ColorAttribute] = colors;
    m_nComponents[PrivateShaderDefs::TextureCoordinateAttribute] = texCoords;
}

VertexFormat::VertexFormat()
    : VertexFormat(0,0,0,0)
{
}

int VertexFormat::components(PrivateShaderDefs::VertexArrayAttribute attribute) const
{
    if ( attribute < 0 || attribute >= PrivateShaderDefs::VertexAttributeLocationCount )
    {
        return 0;
    }

    return m_nComponents[attribute];
}

int VertexFormat::positionComponents() const
{
    return components(PrivateShaderDefs::PositionAttribute);
}

int VertexFormat::normalComponents() const
{
    return components(PrivateShaderDefs::NormalAttribute);
}

int VertexFormat::colorComponents() const
{
    return components(PrivateShaderDefs::ColorAttribute);
}

int VertexFormat::textureCoordinateComponents() const
{
    return components(PrivateShaderDefs::TextureCoordinateAttribute);
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
    return m_nComponents[PrivateShaderDefs::PositionAttribute] == other.positionComponents() &&
            m_nComponents[PrivateShaderDefs::NormalAttribute] == other.normalComponents() &&
            m_nComponents[PrivateShaderDefs::ColorAttribute] == other.colorComponents() &&
            m_nComponents[PrivateShaderDefs::TextureCoordinateAttribute] == other.textureCoordinateComponents();
}

bool VertexFormat::operator !=(const VertexFormat& other) const
{
    return !(*this == other);
}

bool VertexFormat::isValid() const
{
    return totalVertexComponents() > 0;
}
