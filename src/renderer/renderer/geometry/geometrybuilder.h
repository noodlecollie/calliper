#ifndef GEOMETRYBUILDER_H
#define GEOMETRYBUILDER_H

#include "renderer_global.h"
#include "renderer/geometry/geometrysection.h"
#include "renderer/shaders/vertexformat.h"
#include "renderer/functors/renderfunctorgroup.h"
#include "renderer/shaders/baseshaderpalette.h"

namespace Renderer
{
    class RENDERERSHARED_EXPORT GeometryBuilder
    {
    public:
        // TODO: May be better to put all of these inputs (certainly the functors/palette) into some container class/struct?
        GeometryBuilder(const RenderFunctorGroup& renderFunctors,
                        BaseShaderPalette* shaderPalette,
                        quint32 materialId,
                        const QMatrix4x4 &modelToWorldMatrix);
        ~GeometryBuilder();

        int sectionCount() const;
        GeometrySection* section(int index);
        const GeometrySection* section(int index) const;
        GeometrySection* createNewSection(quint32 materialId, const QMatrix4x4 &matrix);
        GeometrySection* createNewSection();
        GeometrySection* currentSection();
        const QList<GeometrySection*>& sections() const;

        QMatrix4x4 modelToWorldMatrix() const;
        void setModelToWorldMatrix(const QMatrix4x4 &matrix);

        quint32 materialId() const;
        void setMaterialId(quint32 id);

        IShaderRetrievalFunctor* shaderFunctor() const;
        ITextureRetrievalFunctor* textureFunctor() const;
        IMaterialRetrievalFunctor* materialFunctor() const;
        BaseShaderPalette* shaderPalette() const;

        // Export all data into buffers provided.
        void consolidate(QVector<float> &positions, QVector<float> normals,
                         QVector<float> &colors, QVector<float> &textureCoordiates,
                         QVector<quint32> &indices) const;

        // Checks all sections - they must have no positions or indices.
        bool isEmpty() const;

    private:
        void setAllSectionMatrices();

        QList<GeometrySection*>  m_Sections;
        RenderFunctorGroup m_RenderFunctors;
        BaseShaderPalette* m_pShaderPalette;

        quint32 m_iMaterialId;
        QMatrix4x4  m_matModelToWorld;
    };
}

#endif // GEOMETRYBUILDER_H
