#ifndef DEBUGSCREENSPACESHADER_H
#define DEBUGSCREENSPACESHADER_H

#include "renderer_global.h"
#include "openglshaderprogram.h"

namespace NS_RENDERER
{
    class RENDERERSHARED_EXPORT DebugScreenSpaceShader : public OpenGLShaderProgram
    {
        Q_OBJECT
    public:
        DebugScreenSpaceShader(QObject* parent = 0);
        virtual ~DebugScreenSpaceShader();

        virtual void construct() override;

        // IShaderSpec
        virtual int positionComponents() const override
        {
            return 3;
        }

        virtual int normalComponents() const override
        {
            return 0;
        }

        virtual int colorComponents() const override
        {
            return 4;
        }

        virtual int textureCoordinateComponents() const override
        {
            return 2;
        }

        virtual int maxBatchedItems() const
        {
            return 8;
        }
    };
}

#endif // DEBUGSCREENSPACESHADER_H
