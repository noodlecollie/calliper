#ifndef RENDERMODELBATCHGROUPKEY_H
#define RENDERMODELBATCHGROUPKEY_H

#include "renderer_global.h"
#include <QOpenGLFunctions>
#include <QHash>

namespace Renderer
{
    class RENDERERSHARED_EXPORT RenderModelBatchGroupKey
    {
    public:
        RenderModelBatchGroupKey(quint16 shaderId, quint32 materialId, GLenum drawMode = GL_TRIANGLES, float drawWidth = 1.0f);

        quint16 shaderId() const;
        quint32 materialId() const;
        GLenum drawMode() const;
        float drawWidth() const;

        inline bool operator <(const RenderModelBatchGroupKey &other) const
        {
            if ( m_iShaderId != other.m_iShaderId )
                return m_iShaderId < other.m_iShaderId;

            if ( m_iMaterialId != other.m_iMaterialId )
                return m_iMaterialId < other.m_iMaterialId;

            if ( m_iDrawMode != other.m_iDrawMode )
                return m_iDrawMode < other.m_iDrawMode;

            return m_flDrawWidth < other.m_flDrawWidth;
        }

        inline bool operator ==(const RenderModelBatchGroupKey &other) const
        {
            return m_iShaderId == other.m_iShaderId &&
                    m_iMaterialId == other.m_iMaterialId &&
                    m_iDrawMode == other.m_iDrawMode &&
                    m_flDrawWidth == other.m_flDrawWidth;
        }

        inline bool operator !=(const RenderModelBatchGroupKey &other) const
        {
            return !(*this == other);
        }

    private:
        quint16       m_iShaderId;
        quint32       m_iMaterialId;
        GLenum        m_iDrawMode;
        float         m_flDrawWidth;
    };

    RENDERERSHARED_EXPORT uint qHash(const RenderModelBatchGroupKey &key, uint seed = 0);
}

#endif // RENDERMODELBATCHGROUPKEY_H
