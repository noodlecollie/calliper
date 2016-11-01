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
        RendererInputObjectParams(quint32 objectId, int passIndex, const GeometryBuilder &builder);

        const QList<GeometrySection*>& geometrySectionList() const;
        quint32 objectId() const;
        int passIndex() const;

    private:
        const quint32 m_iObjectId;
        const int m_iPassIndex;
        const QList<GeometrySection*>&   m_SectionListReference;
    };
}

#endif // RENDERERINPUTOBJECTPARAMS_H
