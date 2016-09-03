#ifndef RENDERMODELBATCHKEY_H
#define RENDERMODELBATCHKEY_H

#include "renderer_global.h"
#include <QtOpenGL>
#include <QHash>

namespace NS_RENDERER
{
    class RENDERERSHARED_EXPORT RenderModelBatchKey
    {
    public:
        RenderModelBatchKey(quint16 shaderStoreId, quint32 textureId, GLenum drawMode = GL_TRIANGLES, float drawWidth = 1.0f);
        RenderModelBatchKey(const RenderModelBatchKey &other);

        inline bool operator ==(const RenderModelBatchKey &other) const
        {
            return m_iShaderStoreId == other.m_iShaderStoreId &&
                    m_iTextureId == other.m_iTextureId &&
                    m_iDrawMode == other.m_iDrawMode &&
                    m_flDrawWidth == other.m_flDrawWidth;
        }

        /* Used for things like QMap to order keys.
         * Our objective is to minimise state changes so we order keys in a certain way.
         * We can group by processing attributes as follows:
         *
         * - If attribute k1.A < attribute k2.A, k1 is ordered less than k2 (and vice versa).
         * - If k1.A == k2.A, perform the same test with k1.B and k2.B. Continue this across
         *   all attributes.
         *
         * The attributes we want to group on first are checked first. The ordering is
         * as follows:
         *
         * - Shader (expensive to change, expected bias towards ShadedTextured shader)
         * - Texture (a little expensive to change, no a priori bias)
         * - Draw mode (not very expensive to change, but huge bias towards GL_TRIANGLES)
         */
        inline bool operator <(const RenderModelBatchKey &other) const
        {
            if ( m_iShaderStoreId != other.m_iShaderStoreId )
                return m_iShaderStoreId < other.m_iShaderStoreId;

            if ( m_iTextureId != other.m_iTextureId )
                return m_iTextureId < other.m_iTextureId;

            if ( m_iDrawMode != other.m_iDrawMode )
                return m_iDrawMode < other.m_iDrawMode;

            return m_flDrawWidth < other.m_flDrawWidth;
        }

        quint16 shaderStoreId() const;
        quint32 textureId() const;
        GLenum drawMode() const;
        float drawWidth() const;

    private:
        quint16     m_iShaderStoreId;
        quint32     m_iTextureId;
        GLenum      m_iDrawMode;
        float       m_flDrawWidth;
    };

    uint RENDERERSHARED_EXPORT qHash(const RenderModelBatchKey &key, uint seed);
}

#endif // RENDERMODELBATCHKEY_H
