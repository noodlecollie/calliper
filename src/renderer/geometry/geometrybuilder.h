#ifndef GEOMETRYBUILDER_H
#define GEOMETRYBUILDER_H

#include "renderer_global.h"
#include "geometry/geometrysection.h"
#include "shaders/vertexformat.h"
#include "functors/ishaderretrievalfunctor.h"
#include "functors/itextureretrievalfunctor.h"

namespace Renderer
{
    class RENDERERSHARED_EXPORT GeometryBuilder
    {
    public:
        GeometryBuilder(IShaderRetrievalFunctor* shaderFunctor, ITextureRetrievalFunctor* textureFunctor,
                        quint16 shaderId, quint32 materialId, const QMatrix4x4 &modelToWorldMatrix);
        ~GeometryBuilder();

        int sectionCount() const;
        GeometrySection* section(int index);
        const GeometrySection* section(int index) const;
        GeometrySection* createNewSection(quint16 shaderId, quint32 materialId, const QMatrix4x4 &matrix);
        GeometrySection* createNewSection();
        GeometrySection* currentSection();
        const QList<GeometrySection*>& sections() const;

        QMatrix4x4 modelToWorldMatrix() const;
        void setModelToWorldMatrix(const QMatrix4x4 &matrix);

        quint16 shaderId() const;
        void setShaderId(quint16 id);

        quint32 materialId() const;
        void setMaterialId(quint32 id);

        IShaderRetrievalFunctor* shaderFunctor() const;
        ITextureRetrievalFunctor* textureFunctor() const;

        // Export all data into buffers provided.
        void consolidate(QVector<float> &positions, QVector<float> normals,
                         QVector<float> &colors, QVector<float> &textureCoordiates,
                         QVector<quint32> &indices) const;

        // Checks all sections - they must have no positions or indices.
        bool isEmpty() const;

    private:
        void setAllSectionMatrices();

        QList<GeometrySection*>  m_Sections;
        IShaderRetrievalFunctor* m_pShaderFunctor;
        ITextureRetrievalFunctor* m_pTextureFunctor;

        quint16 m_iShaderId;
        quint32 m_iMaterialId;
        QMatrix4x4  m_matModelToWorld;
    };
}

#endif // GEOMETRYBUILDER_H
