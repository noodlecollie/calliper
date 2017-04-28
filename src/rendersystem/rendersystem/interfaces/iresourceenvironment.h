#ifndef IRESOURCEENVIRONMENT_H
#define IRESOURCEENVIRONMENT_H

#include "rendersystem_global.h"

namespace RenderSystem
{
    class IResourceEnvironment
    {
    public:
        virtual ~IResourceEnvironment() {}

        // Stores:
        // - Shader
        // - Texture
        // - Render style (maps draw modes to shaders)
        // - Material (references a shader, maps from texture units to textures, provides other metadata)
    };
}

#endif // IRESOURCEENVIRONMENT_H
