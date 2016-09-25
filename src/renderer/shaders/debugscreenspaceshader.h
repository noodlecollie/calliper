#ifndef DEBUGSCREENSPACESHADER_H
#define DEBUGSCREENSPACESHADER_H

#include "renderer_global.h"
#include "opengl/openglshaderprogram.h"

namespace NS_RENDERER
{
    class RENDERERSHARED_EXPORT DebugScreenSpaceShader : public OpenGLShaderProgram
    {
        Q_OBJECT
    public:
        DebugScreenSpaceShader(quint16 id, QObject* parent = 0);
        virtual ~DebugScreenSpaceShader();

        virtual void construct() override;

        virtual VertexFormat vertexFormat() const override
        {
            return VertexFormat(3, 0, 4, 2);
        }

        virtual int maxBatchedItems() const override
        {
            return 8;
        }
    };
}

#endif // DEBUGSCREENSPACESHADER_H
