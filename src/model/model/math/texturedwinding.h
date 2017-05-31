#ifndef TEXTUREDWINDING_H
#define TEXTUREDWINDING_H

#include "model_global.h"
#include "winding3d.h"

#include "rendersystem/interface-classes/store/publicstoredefs.h"

namespace Model
{
    class MODELSHARED_EXPORT TexturedWinding : public Winding3D
    {
    public:
        TexturedWinding(const Plane3D &plane, RenderSystem::PublicStoreDefs::MaterialId materialId);

        RenderSystem::PublicStoreDefs::MaterialId materialId() const;
        void setMaterialId(RenderSystem::PublicStoreDefs::MaterialId id);

    private:
        RenderSystem::PublicStoreDefs::MaterialId m_iMaterialId;
    };
}

#endif // TEXTUREDWINDING_H
