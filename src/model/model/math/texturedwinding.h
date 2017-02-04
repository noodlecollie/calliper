#ifndef TEXTUREDWINDING_H
#define TEXTUREDWINDING_H

#include "model_global.h"
#include "winding3d.h"

namespace Model
{
    class MODELSHARED_EXPORT TexturedWinding : public Winding3D
    {
    public:
        TexturedWinding(const Plane3D &plane, quint32 materialId);

        quint32 materialId() const;
        void setMaterialId(quint32 id);

    private:
        quint32     m_iMaterialId;
    };
}

#endif // TEXTUREDWINDING_H
