#ifndef PRIVATESHADERDEFS_H
#define PRIVATESHADERDEFS_H

#include <QObject>

#include "rendersystem/interface-classes/definitions/shaderdefs.h"

class VertexFormat;

namespace PrivateShaderDefs
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

        TOTAL_VERTEX_ARRAY_ATTRIBUTES
    };

    enum UniformBlockBindingPoint
    {
        GlobalUniformBlockBindingPoint = 0, // Camera matrices, fog, colour, etc.
        LocalUniformBlockBindingPoint  = 1, // Model to world matrices
    };

    extern const char* GLOBAL_UNIFORM_BLOCK_NAME;
    extern const char* LOCAL_UNIFORM_BLOCK_NAME;
};

#endif // PRIVATESHADERDEFS_H
