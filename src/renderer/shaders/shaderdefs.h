#ifndef SHADERDEFS_H
#define SHADERDEFS_H

#include "renderer_global.h"
#include <QObject>

namespace NS_RENDERER
{
    class RENDERERSHARED_EXPORT ShaderDefs
    {
        Q_GADGET
        ShaderDefs() = delete;
    public:
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
        Q_ENUM(VertexArrayAttribute)

        // Canonical locations for uniforms.
        // TODO: Do we need these if we're using UBOs?
        enum Uniform
        {
            WorldCameraMatrixUniform            = 0,
            CoordinateTransformMatrixUniform    = 1,
            ProjectionMatrixUniform             = 2,

            FogBeginUniform                     = 3,
            FogEndUniform                       = 4,
            FogColorUniform                     = 5,
            CounterScaleUniform                 = 6,
            GlobalColorUniform                  = 7,
        };
        Q_ENUM(Uniform)

        enum UniformBlockBindingPoint
        {
            GlobalUniformBlockBindingPoint = 0, // Camera matrices, fog, colour, etc.
            LocalUniformBlockBindingPoint = 1   // Model to world matrices
        };
        Q_ENUM(UniformBlockBindingPoint)

        static const char* GLOBAL_UNIFORM_BLOCK_NAME;
        static const char* LOCAL_UNIFORM_BLOCK_NAME;
    };
}

#endif // SHADERDEFS_H
