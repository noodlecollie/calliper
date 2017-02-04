#ifndef MATERIALRESOURCEMANAGER_H
#define MATERIALRESOURCEMANAGER_H

#include "model_global.h"
#include "materialstore.h"
#include "texturestore.h"

namespace Model
{
    class MODELSHARED_EXPORT MaterialResourceManager
    {
    public:
        MaterialResourceManager();

        const MaterialStore* materialStore() const;
        MaterialStore* materialStore();

        const TextureStore* textureStore() const;
        TextureStore* textureStore();

    private:
        MaterialStore m_MaterialStore;
        TextureStore m_TextureStore;
    };
}

#endif // MATERIALRESOURCEMANAGER_H
