#ifndef RENDERMODELBATCHKEY_H
#define RENDERMODELBATCHKEY_H

#include "renderer_global.h"
#include <QtOpenGL>
#include <QHash>

namespace NS_RENDERER
{
    class RenderModelBatchKey
    {
    public:
        RenderModelBatchKey(quint16 shaderId, quint32 textureId, GLenum drawMode = GL_TRIANGLES, float drawWidth = 1.0f);

        quint16 shaderId() const;
        quint32 textureId() const;
        GLenum drawMode() const;
        float drawWidth() const;

    private:
        quint16     m_iShaderId;
        quint32     m_iTextureId;
        GLenum      m_iDrawMode;
        float       m_flDrawWidth;
    };
}

uint qHash(const NS_RENDERER::RenderModelBatchKey &key, uint seed);

#endif // RENDERMODELBATCHKEY_H
