#include "materialresourcemanager.h"

namespace Model
{
    MaterialResourceManager::MaterialResourceManager()
    {

    }

    const MaterialStore* MaterialResourceManager::materialStore() const
    {
        return &m_MaterialStore;
    }

    MaterialStore* MaterialResourceManager::materialStore()
    {
        return &m_MaterialStore;
    }

    const TextureStore* MaterialResourceManager::textureStore() const
    {
        return &m_TextureStore;
    }

    TextureStore* MaterialResourceManager::textureStore()
    {
        return &m_TextureStore;
    }
}
