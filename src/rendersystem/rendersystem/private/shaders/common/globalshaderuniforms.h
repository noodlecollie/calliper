#ifndef GLOBALSHADERUNIFORMS_H
#define GLOBALSHADERUNIFORMS_H

#include <QMatrix4x4>
#include <QVector3D>

#include "rendersystem/private/opengl/opengluniformbuffer.h"

class GlobalShaderUniforms
{
public:
    GlobalShaderUniforms(QOpenGLBuffer::UsagePattern usagePattern);
    ~GlobalShaderUniforms();

    void create();
    void destroy();

    // Matrix is in Hammer space.
    QMatrix4x4 worldToCameraMatrix() const;
    void setWorldToCameraMatrix(const QMatrix4x4 &mat);

    // Matrix is in OpenGL space.
    QMatrix4x4 projectionMatrix() const;
    void setProjectionMatrix(const QMatrix4x4 &mat);

    // Vector is in Hammer space.
    QVector3D directionalLight() const;
    void setDirectionalLight(const QVector3D &vec);

    bool needsUpload() const;
    void upload();

private:
    bool m_bNeedsUpload;
    OpenGLUniformBuffer m_UniformBuffer;

    QMatrix4x4  m_matWorldToCamera;
    QMatrix4x4  m_matProjection;
    QVector3D   m_vecDirectionalLight;
};

#endif // GLOBALSHADERUNIFORMS_H
