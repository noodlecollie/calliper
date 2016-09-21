#ifndef RENDERERINPUTOBJECTPARAMS_H
#define RENDERERINPUTOBJECTPARAMS_H

#include "renderer_global.h"
#include "rendermodel/0-modellevel/rendermodelkey.h"
#include "geometry/geometrysection.h"
#include "geometry/geometrybuilder.h"
#include <QList>

namespace NS_RENDERER
{
    class RENDERERSHARED_EXPORT RendererInputObjectParams
    {
    public:
        RendererInputObjectParams(const RenderModelKey &modelKey,
                                  const QList<GeometrySection> &geometry);

        RendererInputObjectParams(const RenderModelKey &modelKey,
                                  const GeometryBuilder &builder);

        const RenderModelKey& renderModelKey() const;
        const QList<GeometrySection>& geometrySectionList() const;

    private:
        const RenderModelKey            m_RenderModelKey;
        const QList<GeometrySection>&   m_SectionListReference;
    };
}

#endif // RENDERERINPUTOBJECTPARAMS_H
