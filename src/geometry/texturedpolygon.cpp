#include "texturedpolygon.h"

TexturedPolygon::TexturedPolygon(const Plane3D &plane, const QString &tex) :
    Winding3D(plane), m_szTexture(tex)
{

}

QString TexturedPolygon::texture() const
{
    return m_szTexture;
}

void TexturedPolygon::setTexture(const QString &str)
{
    m_szTexture = str;
}
