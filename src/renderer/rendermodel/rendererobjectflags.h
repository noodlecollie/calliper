#ifndef RENDEREROBJECTFLAGS_H
#define RENDEREROBJECTFLAGS_H

#include "renderer_global.h"

namespace Renderer
{
    enum ObjectFlags
    {
        NoObjectFlag = 0,

        HiddenObjectFlag = (1<<0),
    };
}

#endif // RENDEREROBJECTFLAGS_H
