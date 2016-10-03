#ifndef GEOMETRYBUILDER_H
#define GEOMETRYBUILDER_H

#include "renderer_global.h"
#include "geometry/geometrysection.h"

namespace NS_RENDERER
{
    class RENDERERSHARED_EXPORT GeometryBuilder
    {
    public:
        GeometryBuilder(quint16 shaderId, quint32 textureId, const QMatrix4x4 &modelToWorldMatrix);
        ~GeometryBuilder();

        int sectionCount() const;
        GeometrySection& section(int index);
        const GeometrySection& section(int index) const;
        GeometrySection& createNewSection(quint16 shaderId, quint32 textureId, const QMatrix4x4 &matrix);
        GeometrySection& createNewSection();
        GeometrySection& nextEmptySection();
        GeometrySection& currentSection();
        const QList<GeometrySection>& sections() const;

        QMatrix4x4 modelToWorldMatrix() const;
        void setModelToWorldMatrix(const QMatrix4x4 &matrix);

        quint16 shaderId() const;
        void setShaderId(quint16 id);

        quint32 textureId() const;
        void setTextureId(quint32 id);

        // Export all data into buffers provided.
        void consolidate(QVector<float> &positions, QVector<float> normals,
                         QVector<float> &colors, QVector<float> &textureCoordiates,
                         QVector<quint32> &indices) const;

    private:
        void setAllSectionMatrices();

        QList<GeometrySection>  m_Sections;
        quint16 m_iShaderId;
        quint32 m_iTextureId;
        QMatrix4x4  m_matModelToWorld;
    };
}

#endif // GEOMETRYBUILDER_H
