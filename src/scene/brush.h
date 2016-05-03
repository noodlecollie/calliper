#ifndef BRUSH_H
#define BRUSH_H

#include "sceneobject.h"

class BrushFace;

class Brush : public SceneObject
{
    Q_OBJECT
    friend class BaseScene;
public:
    virtual SceneObject* clone() const;

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

    virtual float computeIntersection(const Ray3D &ray, QRgb *col = NULL, RayCoordinateSpace space = IRayDetectable::WorldSpace) const;

protected:
    explicit Brush(BaseScene* scene, SceneObject* parent);
    Brush(BaseScene* scene, const QJsonObject &serialisedData, SceneObject* parent);
    Brush(const Brush &cloneFrom);
    virtual ~Brush();

private:
    void initDefaults();
    void removeVertexFromChildFaces(int index);
    void clearVerticesFromChildFaces();
    void updateVertexInChildFaces(int index);

    QVector<QVector3D>    m_Vertices;
};

#endif // BRUSH_H
