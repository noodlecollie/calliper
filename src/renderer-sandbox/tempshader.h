#ifndef TEMPSHADER_H
#define TEMPSHADER_H

#include "opengl/openglshaderprogram.h"

class TempShader : public NS_RENDERER::OpenGLShaderProgram
{
    Q_OBJECT
public:
    TempShader(QObject* parent = 0);
    virtual ~TempShader();

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

    virtual int maxBatchedItems() const override
    {
        return 8;
    }
};

#endif // TEMPSHADER_H
