#ifndef FRAMEDRAWPARAMS_H
#define FRAMEDRAWPARAMS_H

#include "rendersystem_global.h"

#include <QMatrix4x4>
#include <QVector3D>
#include <QColor>

#include "rendersystem/interface-classes/definitions/shaderdefs.h"

namespace RenderSystem
{
    // Used to hold parameters that should be constant across an entire frame,
    // and which are owned by objects outside of a render model.
    class RENDERSYSTEMSHARED_EXPORT FrameDrawParams
    {
    public:
        FrameDrawParams();

        RenderSystem::ShaderDefs::RenderMode renderMode() const;
        void setRenderMode(RenderSystem::ShaderDefs::RenderMode mode);

        const QMatrix4x4& worldToCameraMatrix() const;
        void setWorldToCameraMatrix(const QMatrix4x4& mat);

        const QMatrix4x4& projectionMatrix() const;
        void setProjectionMatrix(const QMatrix4x4& mat);

        QColor backgroundColor() const;
        void setBackgroundColor(const QColor& col);

        // Will be normalised automatically when rendering.
        QVector3D directionalLight() const;
        void setDirectionalLight(const QVector3D& dir);

    private:
        RenderSystem::ShaderDefs::RenderMode m_nRenderMode;
        QMatrix4x4 m_matWorldToCamera;
        QMatrix4x4 m_matProjection;
        QColor m_colBackground;
        QVector3D m_vecDirectionalLight;
    };
}

#endif // FRAMEDRAWPARAMS_H
