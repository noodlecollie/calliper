#ifndef ISHADERSPEC_H
#define ISHADERSPEC_H

#include "renderer_global.h"

namespace NS_RENDERER
{
    class IShaderSpec
    {
    public:
        virtual ~IShaderSpec() {}

        // Each attribute can have a maximum of 4 components,
        // as each shader location is a vec4.
        virtual int positionComponents() const = 0;
        virtual int normalComponents() const = 0;
        virtual int colorComponents() const = 0;
        virtual int textureCoordinateComponents() const = 0;

        inline int totalVertexComponents() const
        {
            return positionComponents()
                    + normalComponents()
                    + colorComponents()
                    + textureCoordinateComponents();
        }
    };
}

#endif // ISHADERSPEC_H
