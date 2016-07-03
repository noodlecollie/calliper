#include "texturedpolygon.h"

TexturedPolygon::TexturedPolygon(const Plane3D &plane, const QString &tex) :
    m_Winding(plane), m_szTexture(tex)
{

}

Winding3D& TexturedPolygon::winding()
{
    return m_Winding;
}

const Winding3D& TexturedPolygon::winding() const
{
    return m_Winding;
}

QString TexturedPolygon::texture() const
{
    return m_szTexture;
}

void TexturedPolygon::setTexture(const QString &str)
{
    m_szTexture = str;
}
