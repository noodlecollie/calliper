#include "rendererinputobjectparams.h"

namespace NS_RENDERER
{
    RendererInputObjectParams::RendererInputObjectParams(const RenderModelKey &modelKey,
                                                         const QList<GeometrySection> &geometry)
        : m_RenderModelKey(modelKey),
          m_SectionListReference(geometry)
    {

    }

    RendererInputObjectParams::RendererInputObjectParams(const RenderModelKey &modelKey,
                                                         const GeometryBuilder &builder)
        : m_RenderModelKey(modelKey),
          m_SectionListReference(builder.sections())
    {

    }

    const RenderModelKey& RendererInputObjectParams::renderModelKey() const
    {
        return m_RenderModelKey;
    }

    const QList<GeometrySection>& RendererInputObjectParams::geometrySectionList() const
    {
        return m_SectionListReference;
    }
}
