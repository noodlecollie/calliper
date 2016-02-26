#ifndef BRUSHFACE_H
#define BRUSHFACE_H

#include "sceneobject.h"

class Brush;
class TexturePlane;

class BrushFace : public SceneObject
{
    Q_OBJECT
    friend class Brush;
public:
    explicit BrushFace(Brush* parent, QList<int> vertices = QList<int>());
    virtual ~BrushFace();

    Brush* parentBrush() const;

    int vertexAt(int index) const;
    void appendVertex(int v);
    int vertexCount() const;
    void removeVertex(int index);
    QList<int> vertexList() const;
    void clearVertices();
    QList<QVector3D> referencedVertexList() const;

    bool isValid() const;
    QVector3D normal() const;

    TexturePlane* texturePlane() const;

    virtual void draw(ShaderStack *stack);

private slots:
    void forceRebuildGeometry();

private:
    void notifyVertexRemoved(int index);
    void notifyVertexChanged(int index);
    void buildGeometry();

    QList<int>      m_Vertices;
    bool            m_bRebuildGeometry;
    TexturePlane*   m_pTexturePlane;
};

#endif // BRUSHFACE_H
