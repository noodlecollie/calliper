#ifndef GLOBALSHADERUNIFORMS_H
#define GLOBALSHADERUNIFORMS_H

#include "renderer_global.h"
#include <QMatrix4x4>
#include <QVector3D>
#include "opengl/opengluniformbuffer.h"

namespace NS_RENDERER
{
    class GlobalShaderUniforms
    {
    public:
        GlobalShaderUniforms(QOpenGLBuffer::UsagePattern usagePattern);
        ~GlobalShaderUniforms();

        void create();
        void destroy();

        // Hammer space
        QMatrix4x4 worldToCameraMatrix() const;
        void setWorldToCameraMatrix(const QMatrix4x4 &mat);

        // OpenGL space
        QMatrix4x4 projectionMatrix() const;
        void setProjectionMatrix(const QMatrix4x4 &mat);

        // Hammer space
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
}

#endif // GLOBALSHADERUNIFORMS_H
