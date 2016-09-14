#ifndef RENDERMODELINPUTPARAMS_H
#define RENDERMODELINPUTPARAMS_H

#include "renderer_global.h"
#include <QMatrix4x4>
#include <QOpenGLFunctions>
#include <QList>
#include "geometry/geometrysection.h"

namespace NS_RENDERER
{
    class RENDERERSHARED_EXPORT RenderModelInputParams
    {
    public:
        RenderModelInputParams(int passIndex, quint32 shaderId, quint64 textureId, const QMatrix4x4 &modelToWorld,
                               GLenum drawMode = GL_TRIANGLES, float drawWidth = 1.0f);
        RenderModelInputParams(const RenderModelInputParams &other);

        int passIndex() const;
        quint32 shaderId() const;
        quint64 textureId() const;
        const QMatrix4x4& modelToWorldMatrix() const;
        GLenum drawMode() const;
        float drawWidth() const;

        inline bool operator ==(const RenderModelInputParams &other) const
        {
            return m_iPassIndex == other.m_iPassIndex &&
                    m_iShaderId == other.m_iShaderId &&
                    m_iTextureId == other.m_iTextureId &&
                    m_matModelToWorld == other.m_matModelToWorld &&
                    m_iDrawMode == other.m_iDrawMode &&
                    m_flDrawWidth == other.m_flDrawWidth;
        }

        inline bool operator !=(const RenderModelInputParams &other) const
        {
            return !(*this == other);
        }

    private:
        int         m_iPassIndex;
        quint32     m_iShaderId;
        quint64     m_iTextureId;
        QMatrix4x4  m_matModelToWorld;
        GLenum      m_iDrawMode;
        float       m_flDrawWidth;
    };
}

#endif // RENDERMODELINPUTPARAMS_H
