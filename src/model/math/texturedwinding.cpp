#include "texturedwinding.h"

namespace Model
{
    TexturedWinding::TexturedWinding(const Plane3D &plane, quint32 textureId) :
        Winding3D(plane), m_iTextureId(textureId)
    {

    }

    quint32 TexturedWinding::textureId() const
    {
        return m_iTextureId;
    }

    void TexturedWinding::setTextureId(quint32 id)
    {
        m_iTextureId = id;
    }
}
