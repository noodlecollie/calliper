#ifndef RENDERMODELDEFS_H
#define RENDERMODELDEFS_H

#include "rendersystem_global.h"

namespace RenderSystem
{
    namespace RenderModelDefs
    {
        typedef quint32 ObjectId;
        typedef quint8 SectionId;
        typedef quint16 RenderModelId;

        RENDERSYSTEMSHARED_EXPORT extern const ObjectId INVALID_OBJECT_ID;
        RENDERSYSTEMSHARED_EXPORT extern const RenderModelId INVALID_RENDER_MODEL_ID;

        enum PresetRenderModel
        {
            ScreenSpaceQuad = 0,

            TOTAL_PRESET_RENDER_MODELS
        };
    }
}

#endif // RENDERMODELDEFS_H
