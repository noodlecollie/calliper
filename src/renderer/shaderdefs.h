#ifndef SHADERDEFS_H
#define SHADERDEFS_H

#include "renderer_global.h"

namespace NS_RENDERER
{
    namespace ShaderDefs
    {
        // Canonical array IDs for vertex attributes.
        // This should be kept below MAX_VERTEX_ATTRIBS.
        // Each array corresponds to a location in the
        // vertex shader, and the attribute can use
        // up to a vec4 per location.
        enum VertexArrayAttribute
        {
            PositionAttribute           = 0,
            NormalAttribute             = 1,
            ColorAttribute              = 2,
            TextureCoordinateAttribute  = 3,

            VertexAttributeLocationCount
        };

        // Canonical locations for uniforms.
        enum Uniform
        {
            // TODO: Change these to take into account multiple locations for matrices
            UniformStartLocation = 0,

            WorldCameraMatrixUniform            = 0,
            CoordinateTransformMatrixUniform    = 1,
            ProjectionMatrixUniform             = 2,

            FogBeginUniform                     = 3,
            FogEndUniform                       = 4,
            FogColorUniform                     = 5,
            CounterScaleUniform                 = 6,
            GlobalColorUniform                  = 7,
        };

        // Canonical locations for uniform arrays.
        // These take up multiple slots.
        enum UniformArrays
        {
            ArrayUniformStartLocation = 16,

            ModelWorldMatrixArrayUniform = 16,
            ModelWorldMatrixArrayUniformLast = 47,   // 8 * 4 = 32 locations used

            UniformLocationCount
        };
    }
}

#endif // SHADERDEFS_H
