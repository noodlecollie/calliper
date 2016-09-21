#ifndef RENDERMODELBATCHGROUPKEY_H
#define RENDERMODELBATCHGROUPKEY_H

#include "renderer_global.h"
#include <QOpenGLFunctions>
#include <QHash>

namespace NS_RENDERER
{
    class RENDERERSHARED_EXPORT RenderModelBatchGroupKey
    {
    public:
        RenderModelBatchGroupKey(quint16 shaderId, quint16 textureId, GLenum drawMode = GL_TRIANGLES, float drawWidth = 1.0f);

        quint16 shaderId() const;
        quint32 textureId() const;
        GLenum drawMode() const;
        float drawWidth() const;

        inline bool operator <(const RenderModelBatchGroupKey &other) const
        {
            if ( m_iShaderId != other.m_iShaderId )
                return m_iShaderId < other.m_iShaderId;

            if ( m_iTextureId != other.m_iTextureId )
                return m_iTextureId < other.m_iTextureId;

            if ( m_iDrawMode != other.m_iDrawMode )
                return m_iDrawMode < other.m_iDrawMode;

            return m_flDrawWidth < other.m_flDrawWidth;
        }

        inline bool operator ==(const RenderModelBatchGroupKey &other) const
        {
            return m_iShaderId == other.m_iShaderId &&
                    m_iTextureId == other.m_iTextureId &&
                    m_iDrawMode == other.m_iDrawMode &&
                    m_flDrawWidth == other.m_flDrawWidth;
        }

        inline bool operator !=(const RenderModelBatchGroupKey &other) const
        {
            return !(*this == other);
        }

    private:
        const quint16       m_iShaderId;
        const quint32       m_iTextureId;
        const GLenum        m_iDrawMode;
        const float         m_flDrawWidth;
    };

    RENDERERSHARED_EXPORT uint qHash(const RenderModelBatchGroupKey &key, uint seed = 0);
}

#endif // RENDERMODELBATCHGROUPKEY_H
