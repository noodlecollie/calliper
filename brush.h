#ifndef BRUSH_H
#define BRUSH_H

#include "sceneobject.h"

class BrushFace;

class Brush : public SceneObject
{
    Q_OBJECT
public:
    explicit Brush(SceneObject* parent = 0);
    virtual ~Brush();

    QVector3D vertexAt(int index) const;
    int appendVertex(const QVector3D &v);
    int vertexCount() const;
    void removeVertex(int index);
    QList<QVector3D> vertexList() const;
    void clearVertices();
    QList<QVector3D> vertexList(const QList<int> &indices) const;

    QList<BrushFace*> faces() const;

private:
    void removeVertexFromChildFaces(int index);
    void clearVerticesFromChildFaces();

    QList<QVector3D>    m_Vertices;
};

#endif // BRUSH_H
