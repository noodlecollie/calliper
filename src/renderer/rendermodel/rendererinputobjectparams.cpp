#include "rendererinputobjectparams.h"

namespace Renderer
{
    RendererInputObjectParams::RendererInputObjectParams(quint32 objectId, int passIndex, const GeometryBuilder &builder)
        : m_iObjectId(objectId), m_iPassIndex(passIndex), m_SectionListReference(builder.sections())
    {

    }

    const QList<GeometrySection*>& RendererInputObjectParams::geometrySectionList() const
    {
        return m_SectionListReference;
    }

    quint32 RendererInputObjectParams::objectId() const
    {
        return m_iObjectId;
    }

    int RendererInputObjectParams::passIndex() const
    {
        return m_iPassIndex;
    }
}
