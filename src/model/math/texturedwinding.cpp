#include "texturedwinding.h"

namespace Model
{
    TexturedWinding::TexturedWinding(const Plane3D &plane, quint32 materialId) :
        Winding3D(plane), m_iMaterialId(materialId)
    {

    }

    quint32 TexturedWinding::materialId() const
    {
        return m_iMaterialId;
    }

    void TexturedWinding::setMaterialId(quint32 id)
    {
        m_iMaterialId = id;
    }
}
