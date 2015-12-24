#ifndef UNLITTEXTURESHADER_H
#define UNLITTEXTURESHADER_H

#include "shaderprogram.h"
#include <QMatrix4x4>

class UnlitTextureShader : public ShaderProgram
{
public:
    UnlitTextureShader();
    virtual void construct();
    virtual void apply() const;
    virtual void release() const;
    virtual void setModelToWorld(const QMatrix4x4 &mat);
    virtual void setWorldToCamera(const QMatrix4x4 &mat);
    virtual void setCameraProjection(const QMatrix4x4 &mat);

private:
    GLuint  m_iPositionLocation;
    GLuint  m_iUVLocation;
    GLuint  m_iModelToWorldLocation;
    GLuint  m_iWorldToCameraLocation;
    GLuint  m_iHammerToOpenGLLocation;
    GLuint  m_iProjectionLocation;
    QMatrix4x4  m_matModelToWorld;
    QMatrix4x4  m_matWorldToCamera;
    QMatrix4x4  m_matCameraProjection;
};

#endif // UNLITTEXTURESHADER_H
