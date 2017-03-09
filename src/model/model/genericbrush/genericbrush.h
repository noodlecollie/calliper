#ifndef GENERICBRUSH_H
#define GENERICBRUSH_H

#include "model_global.h"
#include "model/scene/sceneobject.h"
#include <QVector>
#include <QVector3D>
#include "genericbrushface.h"
#include <QSharedPointer>

namespace Model
{
    class MODELSHARED_EXPORT GenericBrush : public SceneObject
    {
        friend class Scene;
        Q_OBJECT
    public:
        // Faces/edges/etc are independent of the vertices.
        // Removing vertices here may cause faces or edges to become invalid!
        QVector3D brushVertexAt(int index) const;
        int appendBrushVertex(const QVector3D &v);
        void appendBrushVertices(const QVector<QVector3D> &verts);
        int brushVertexCount() const;
        void removeBrushVertex(int index);
        const QVector<QVector3D>& brushVertexList() const;
        void clearBrushVertices();
        QVector<QVector3D> brushVertexList(const QVector<int> &indices) const;
        void replaceBrushVertex(int index, const QVector3D &v);

        GenericBrushFace* brushFaceAt(int index) const;
        int createBrushFace();
        void destroyBrushFace(int index);
        void clearBrushFaces();
        const QVector<GenericBrushFace*>& brushFaceList() const;
        int brushFaceCount() const;
        GenericBrushFace* createAndObtainBrushFace();

    protected:
        GenericBrush(const SceneObjectInitParams &initParams, SceneObject* parentObject);
        GenericBrush(const GenericBrush* cloneFrom, const SceneObjectInitParams &initParams);
        virtual ~GenericBrush();

        virtual void bakeGeometry(const Renderer::BaseShaderPalette* shaderPalette, Renderer::GeometryBuilder &builder) const;

    private slots:
        void brushFaceUpdated();

    private:
        void commonInit();

        QVector<QVector3D>  m_BrushVertices;
        QVector<GenericBrushFace*>   m_BrushFaces;
    };
}

#endif // GENERICBRUSH_H
