#ifndef SHADERDEFS_H
#define SHADERDEFS_H

#include "renderer_global.h"
#include <QObject>
#include "vertexformat.h"

namespace Renderer
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

        // Different ways in which we might want to draw geometry.
        // Some other examples might be unlit textured, or
        // refract (for water).
        enum ShaderTechnique
        {
            UnlitTextured3D,
            LitTextured3D,
            UnlitPerVertexColor3D,
        };
        Q_ENUM(ShaderTechnique)

        enum UniformBlockBindingPoint
        {
            GlobalUniformBlockBindingPoint = 0, // Camera matrices, fog, colour, etc.
            LocalUniformBlockBindingPoint  = 1, // Model to world matrices
        };
        Q_ENUM(UniformBlockBindingPoint)

        // Canonical texture units for different purposes.
        enum TextureUnit
        {
            MainTexture         = 0,
            SecondaryTexture    = 1,
            NormalMap           = 2,
        };
        Q_ENUM(TextureUnit)

        // This is the maximum number of components required per attribute.
        // Eg. some unlit textured shaders might need normals but most could
        // probably operate without them, so the unlit texture vertex
        // format would specify 3 normal components.
        // In other words, the vertex format returned here puts an upper
        // limit on the number of components shaders that are used for this
        // technique are allowed to ask for.
        typedef VertexFormat VertexFormatUpperBound;
        static VertexFormatUpperBound shaderMaxVertexFormat(ShaderTechnique technique);

        static const char* GLOBAL_UNIFORM_BLOCK_NAME;
        static const char* LOCAL_UNIFORM_BLOCK_NAME;
    };
}

#endif // SHADERDEFS_H
