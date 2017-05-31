#include "texturedwinding.h"

namespace Model
{
    TexturedWinding::TexturedWinding(const Plane3D &plane, quint32 materialId)
        : Winding3D(plane),
        m_iMaterialId(materialId)
    {

    }

    RenderSystem::PublicStoreDefs::MaterialId TexturedWinding::materialId() const
    {
        return m_iMaterialId;
    }

    void TexturedWinding::setMaterialId(RenderSystem::PublicStoreDefs::MaterialId id)
    {
        m_iMaterialId = id;
    }
}
