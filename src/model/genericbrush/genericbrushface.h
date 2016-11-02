#ifndef GENERICBRUSHFACE_H
#define GENERICBRUSHFACE_H

#include "model_global.h"
#include <QVector>
#include <QVector3D>
#include <QObject>
#include "textureplane.h"
#include "geometry/geometrybuilder.h"
#include "shaders/shaderpalette.h"

namespace NS_MODEL
{
    class GenericBrush;

    class MODELSHARED_EXPORT GenericBrushFace : public QObject
    {
        Q_OBJECT
    public:
        GenericBrushFace(GenericBrush* parentBrush);

        GenericBrush* parentBrush() const;

        int indexAt(int index) const;
        void appendIndex(int i);
        void appendIndices(const QVector<int>& indices);
        int indexCount() const;
        void removeIndex(int index);
        QVector<int> indexList() const;
        void clearIndices();
        QVector<QVector3D> referencedBrushVertexList() const;

        TexturePlane* texturePlane() const;

        QVector3D normal() const;

        void buildFaceGeometry(const ShaderPalette& shaderPalette, Renderer::GeometryBuilder &builder) const;

    signals:
        void dataChanged();

    private:
        QVector<int>    m_BrushVertexIndices;
        TexturePlane*   m_pTexturePlane;
    };
}

#endif // GENERICBRUSHFACE_H
