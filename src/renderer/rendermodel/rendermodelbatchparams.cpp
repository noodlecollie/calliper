#include "rendermodelbatchparams.h"

namespace NS_RENDERER
{
    RenderModelBatchParams::RenderModelBatchParams(const QList<GeometrySection> &sections, const QMatrix4x4 &modelWorldMatrix)
        : m_matModelToWorld(modelWorldMatrix), m_Sections(sections),
          m_iVertexCount(0), m_iIndexCount(0), m_bNormalsSpecified(true), m_bColorsSpecified(true), m_bTexCoordsSpecified(true)
    {
        processSectionList();
    }

    const QMatrix4x4& RenderModelBatchParams::modelToWorldMatrix() const
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

    void RenderModelBatchParams::processSectionList()
    {
        foreach ( const GeometrySection &section, m_Sections )
        {
            m_iVertexCount += section.positionCount();
            m_iIndexCount += section.indexCount();

            m_bNormalsSpecified = m_bColorsSpecified && section.hasNormals();
            m_bColorsSpecified = m_bColorsSpecified && section.hasColors();
            m_bTexCoordsSpecified = m_bTexCoordsSpecified && section.hasTextureCoordinates();
        }
    }

    const QList<GeometrySection>& RenderModelBatchParams::sections() const
    {
        return m_Sections;
    }
}
