#ifndef RENDERMODELBATCHPARAMS_H
#define RENDERMODELBATCHPARAMS_H

#include "renderer_global.h"
#include <QMatrix4x4>
#include "geometry/geometrysection.h"

namespace NS_RENDERER
{
    class RENDERERSHARED_EXPORT RenderModelBatchParams
    {
    public:
        RenderModelBatchParams(const QList<GeometrySection> &sections, const QMatrix4x4 &modelWorldMatrix);

        int vertexCount() const;
        int indexCount() const;
        quint32* indices() const;
        const QMatrix4x4& modelToWorldMatrix() const;

        bool someAttributesUnspecified() const;
        bool hasNormals() const;
        bool hasColors() const;
        bool hasTextureCoordinates() const;

        const QList<GeometrySection>& sections() const;

    private:
        void processSectionList();

        QMatrix4x4  m_matModelToWorld;
        const QList<GeometrySection>&   m_Sections;

        int         m_iVertexCount;
        int         m_iIndexCount;
        bool        m_bNormalsSpecified;
        bool        m_bColorsSpecified;
        bool        m_bTexCoordsSpecified;
    };
}

#endif // RENDERMODELBATCHPARAMS_H
