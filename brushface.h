#ifndef BRUSHFACE_H
#define BRUSHFACE_H

#include "sceneobject.h"

class Brush;

class BrushFace : public SceneObject
{
    Q_OBJECT
    friend class Brush;
public:
    explicit BrushFace(Brush* parent, QList<int> vertices = QList<int>());
    virtual ~BrushFace();

    Brush* parentBrush() const;

    QString texturePath() const;
    void setTexturePath(const QString &path);

    int vertexAt(int index) const;
    void appendVertex(int v);
    int vertexCount() const;
    void removeVertex(int index);
    QList<int> vertexList() const;
    void clearVertices();
    QList<QVector3D> referencedVertexList() const;

private:
    void notifyVertexRemoved(int index);

    QString     m_szTexturePath;
    QList<int>  m_Vertices;
};

#endif // BRUSHFACE_H
