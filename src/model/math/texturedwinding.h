#ifndef TEXTUREDWINDING_H
#define TEXTUREDWINDING_H

#include "model_global.h"
#include "winding3d.h"

namespace NS_MODEL
{
    class MODELSHARED_EXPORT TexturedWinding : public Winding3D
    {
    public:
        TexturedWinding(const Plane3D &plane, quint32 textureId);

        quint32 textureId() const;
        void setTextureId(quint32 id);

    private:
        quint32     m_iTextureId;
    };
}

#endif // TEXTUREDWINDING_H
