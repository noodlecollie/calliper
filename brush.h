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
    void appendVertices(const QList<QVector3D> &verts);
    int vertexCount() const;
    void removeVertex(int index);
    QList<QVector3D> vertexList() const;
    void clearVertices();
    QList<QVector3D> vertexList(const QList<int> &indices) const;
    void replaceVertex(int index, const QVector3D &v);

    QList<BrushFace*> faces() const;

private:
    void removeVertexFromChildFaces(int index);
    void clearVerticesFromChildFaces();
    void updateVertexInChildFaces(int index);

    QList<QVector3D>    m_Vertices;
};

#endif // BRUSH_H
