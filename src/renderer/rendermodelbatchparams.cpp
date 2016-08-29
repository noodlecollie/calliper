#include "rendermodelbatchparams.h"

namespace NS_RENDERER
{
    RenderModelBatchParams::RenderModelBatchParams(int vertexCount, float* positions, int indexCount, quint32* indices,
                                                   const QMatrix4x4 &modelToWorld, float* normals, float* colors,
                                                   float* textureCoordinates)
        : m_iVertexCount(vertexCount), m_pPositions(positions), m_pNormals(normals), m_pColors(colors),
          m_pTextureCoordinates(textureCoordinates), m_matModelToWorld(modelToWorld),
          m_iIndexCount(indexCount), m_pIndices(indices)
    {
        Q_ASSERT_X(m_pPositions, Q_FUNC_INFO, "Position data must be specified!");
        Q_ASSERT_X(m_pIndices, Q_FUNC_INFO, "Index data must be specified!");
    }

    float* RenderModelBatchParams::positions() const
    {
        return m_pPositions;
    }

    float* RenderModelBatchParams::normals() const
    {
        return m_pNormals;
    }

    float* RenderModelBatchParams::colors() const
    {
        return m_pColors;
    }

    float* RenderModelBatchParams::textureCoordinates() const
    {
        return m_pTextureCoordinates;
    }

    QMatrix4x4 RenderModelBatchParams::modelToWorldMatrix() const
    {
        return m_matModelToWorld;
    }

    bool RenderModelBatchParams::someAttributesUnspecified() const
    {
        return !(hasNormals() && hasColors() && hasTextureCoordinates());
    }

    bool RenderModelBatchParams::hasNormals() const
    {
        return m_pNormals != NULL;
    }

    bool RenderModelBatchParams::hasColors() const
    {
        return m_pColors != NULL;
    }

    bool RenderModelBatchParams::hasTextureCoordinates() const
    {
        return m_pTextureCoordinates != NULL;
    }

    int RenderModelBatchParams::vertexCount() const
    {
        return m_iVertexCount;
    }

    int RenderModelBatchParams::indexCount() const
    {
        return m_iIndexCount;
    }

    quint32* RenderModelBatchParams::indices() const
    {
        return m_pIndices;
    }
}
