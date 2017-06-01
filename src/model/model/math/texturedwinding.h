#ifndef TEXTUREDWINDING_H
#define TEXTUREDWINDING_H

#include "model_global.h"
#include "winding3d.h"

#include "rendersystem/interface-classes/definitions/materialdefs.h"

namespace Model
{
    class MODELSHARED_EXPORT TexturedWinding : public Winding3D
    {
    public:
        TexturedWinding(const Plane3D &plane, RenderSystem::MaterialDefs::MaterialId materialId);

        RenderSystem::MaterialDefs::MaterialId materialId() const;
        void setMaterialId(RenderSystem::MaterialDefs::MaterialId id);

    private:
        RenderSystem::MaterialDefs::MaterialId m_iMaterialId;
    };
}

#endif // TEXTUREDWINDING_H
