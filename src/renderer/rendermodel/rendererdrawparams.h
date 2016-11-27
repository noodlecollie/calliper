#ifndef RENDERERDRAWPARAMS_H
#define RENDERERDRAWPARAMS_H

#include "renderer_global.h"
#include <QMatrix4x4>

namespace Renderer
{
    class RENDERERSHARED_EXPORT RendererDrawParams
    {
    public:
        RendererDrawParams();

        const QMatrix4x4& worldToCameraMatrix() const;
        void setWorldToCameraMatrix(const QMatrix4x4& mat);

        const QMatrix4x4& projectionMatrix() const;
        void setProjectionMatrix(const QMatrix4x4& mat);

        const QVector3D& directionalLight() const;
        void setDirectionalLight(const QVector3D& vec);

        quint32 defaultTextureId() const;
        void setDefaultTextureId(quint32 id);

        inline bool operator ==(const RendererDrawParams& other) const
        {
            return m_matWorldToCamera == other.m_matWorldToCamera &&
                    m_matProjection == other.m_matProjection &&
                    m_vecDirectionalLight == other.m_vecDirectionalLight;
        }

        inline bool operator !=(const RendererDrawParams& other) const
        {
            return !(*this == other);
        }

    private:
        QMatrix4x4 m_matWorldToCamera;
        QMatrix4x4 m_matProjection;
        QVector3D m_vecDirectionalLight;
        quint32 m_iDefaultTextureId;
    };
}

#endif // RENDERERDRAWPARAMS_H
