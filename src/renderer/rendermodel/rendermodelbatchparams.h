#ifndef RENDERMODELBATCHPARAMS_H
#define RENDERMODELBATCHPARAMS_H

#include "renderer_global.h"
#include <QMatrix4x4>

namespace NS_RENDERER
{
    class RENDERERSHARED_EXPORT RenderModelBatchParams
    {
    public:
        RenderModelBatchParams(int vertexCount, const float* positions, int indexCount, quint32* indices, const QMatrix4x4 &modelToWorld,
                               const float* normals = NULL, const float* colors = NULL, const float* textureCoordinates = NULL);

        int vertexCount() const;
        const float* positions() const;
        const float* normals() const;
        const float* colors() const;
        const float* textureCoordinates() const;
        QMatrix4x4 modelToWorldMatrix() const;
        int indexCount() const;
        quint32* indices() const;

        bool someAttributesUnspecified() const;
        bool hasNormals() const;
        bool hasColors() const;
        bool hasTextureCoordinates() const;

    private:
        int     m_iVertexCount;
        const float*  m_pPositions;
        const float*  m_pNormals;
        const float*  m_pColors;
        const float*  m_pTextureCoordinates;
        QMatrix4x4  m_matModelToWorld;
        int         m_iIndexCount;
        quint32*    m_pIndices;
    };
}

#endif // RENDERMODELBATCHPARAMS_H
