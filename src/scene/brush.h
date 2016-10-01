#ifndef BRUSH_H
#define BRUSH_H

#include "sceneobject.h"
#include <QColor>

class BrushFace;

class Brush : public SceneObject
{
    Q_OBJECT
    friend class BaseScene;
public:
    QVector3D vertexAt(int index) const;
    int appendVertex(const QVector3D &v);
    void appendVertices(const QVector<QVector3D> &verts);
    int vertexCount() const;
    void removeVertex(int index);
    QVector<QVector3D> vertexList() const;
    void clearVertices();
    QVector<QVector3D> vertexList(const QVector<int> &indices) const;
    void replaceVertex(int index, const QVector3D &v);

    QVector<BrushFace*> faces() const;

    virtual bool serialiseToJson(QJsonObject &obj) const;
    virtual QString serialiseIdentifier() const;
    virtual void draw(ShaderStack *stack);
    virtual BoundingBox computeLocalBounds() const;

    virtual bool computeIntersection(const Ray3D &ray, RayTraceContact &contact, RayCoordinateSpace space) const;

    QColor debugColor() const;
    void setDebugColor(const QColor &col);

protected:
    Brush(BaseScene* scene, SceneObject* parent);
    Brush(BaseScene* scene, const QJsonObject &serialisedData, SceneObject* parent);
    Brush(const Brush &cloneFrom);
    virtual ~Brush();

private:
    void initDefaults();
    void removeVertexFromChildFaces(int index);
    void clearVerticesFromChildFaces();
    void updateVertexInChildFaces(int index);

    QVector<QVector3D>    m_Vertices;
    QColor m_colDebugColor;
};

#endif // BRUSH_H
