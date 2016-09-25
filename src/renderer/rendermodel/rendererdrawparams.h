#ifndef RENDERERDRAWPARAMS_H
#define RENDERERDRAWPARAMS_H

#include "renderer_global.h"
#include <QMatrix4x4>

namespace NS_RENDERER
{
    class RENDERERSHARED_EXPORT RendererDrawParams
    {
    public:
        RendererDrawParams();
        RendererDrawParams(const QMatrix4x4 &worldToCameraMatrix, const QMatrix4x4 &projectionMatrix);
        RendererDrawParams(const RendererDrawParams &other);

        const QMatrix4x4& worldToCameraMatrix() const;
        const QMatrix4x4& projectionMatrix() const;

        inline bool operator ==(const RendererDrawParams &other) const
        {
            return m_matWorldToCamera == other.m_matWorldToCamera &&
                    m_matProjection == other.m_matProjection;
        }

        inline bool operator !=(const RendererDrawParams &other) const
        {
            return !(*this == other);
        }

        inline RendererDrawParams& operator =(const RendererDrawParams &other)
        {
            m_matWorldToCamera = other.m_matWorldToCamera;
            m_matProjection = other.m_matProjection;

            return *this;
        }

        int size() const;

    private:
        QMatrix4x4 m_matWorldToCamera;
        QMatrix4x4 m_matProjection;
    };
}

#endif // RENDERERDRAWPARAMS_H
