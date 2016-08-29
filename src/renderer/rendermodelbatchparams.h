#ifndef RENDERMODELBATCHPARAMS_H
#define RENDERMODELBATCHPARAMS_H

#include "renderer_global.h"
#include <QMatrix4x4>

namespace NS_RENDERER
{
    class RenderModelBatchParams
    {
    public:
        RenderModelBatchParams(int vertexCount, float* positions, float* normals, float* colors,
                               float* textureCoordinates, int indexCount, quint32* indices);

        int vertexCount() const;
        float* positions() const;
        float* normals() const;
        float* colors() const;
        float* textureCoordinates() const;
        QMatrix4x4 modelToWorldMatrix() const;
        int indexCount() const;
        quint32* indices() const;

        bool someAttributesUnspecified() const;
        bool hasNormals() const;
        bool hasColors() const;
        bool hasTextureCoordinates() const;

    private:
        int     m_iVertexCount;
        float*  m_pPositions;
        float*  m_pNormals;
        float*  m_pColors;
        float*  m_pTextureCoordinates;
        QMatrix4x4  m_matModelToWorld;
        int         m_iIndexCount;
        quint32*    m_pIndices;
    };
}

#endif // RENDERMODELBATCHPARAMS_H
