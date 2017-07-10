#ifndef PRIVATESHADERDEFS_H
#define PRIVATESHADERDEFS_H

#include <QObject>

#include "rendersystem/interface-classes/definitions/shaderdefs.h"

class VertexFormat;

namespace PrivateShaderDefs
{
    // The shaders we have available.
    // Each entry should correspond to an OpenGLShaderProgram subclass.
    enum ShaderId
    {
        UnknownShaderId = -1,

        ErrorShaderId = 0,
        SimpleLitShaderId,
        UnlitPerVertexColourShaderId,

        TOTAL_SHADERS
    };

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

    enum UniformBlockBindingPoint
    {
        GlobalUniformBlockBindingPoint = 0, // Camera matrices, fog, colour, etc.
        LocalUniformBlockBindingPoint  = 1, // Model to world matrices
    };

    // This is the maximum number of components required per attribute.
    // Eg. some unlit textured shaders might need normals but most could
    // probably operate without them, so the unlit texture vertex
    // format would specify 3 normal components.
    // In other words, the vertex format returned here puts an upper
    // limit on the number of components shaders that are used for this
    // technique are allowed to ask for.
    typedef VertexFormat VertexFormatUpperBound;
    VertexFormatUpperBound shaderMaxVertexFormat(RenderSystem::ShaderDefs::ShaderStyle style);

    extern const char* GLOBAL_UNIFORM_BLOCK_NAME;
    extern const char* LOCAL_UNIFORM_BLOCK_NAME;
};

#endif // PRIVATESHADERDEFS_H
