#ifndef SHADERDEFS_H
#define SHADERDEFS_H

#include "renderer_global.h"

namespace NS_RENDERER
{
    // TODO: Establish canonical locations for all of these.
    namespace ShaderDefs
    {
        enum VertexAttribute
        {
            PositionAttribute,
            NormalAttribute,
            ColorAttribute,
            TextureCoordinateAttribute,
        };

        enum Uniform
        {
            ModelWorldMatrixArrayUniform,
            WorldCameraMatrixUniform,
            CoordinateTransformMatrixUniform,
            ProjectionMatrixUniform,

            FogBeginUniform,
            FogEndUniform,
            FogColorUniform,
            CounterScaleUniform,
            GlobalColorUniform,
        };
    }
}

#endif // SHADERDEFS_H
