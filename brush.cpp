#include "brush.h"
#include "brushface.h"

Brush::Brush(SceneObject *parent) : SceneObject(parent)
{

}

Brush::~Brush()
{
}

QVector3D Brush::vertexAt(int index) const
{
    return m_Vertices.at(index);
}

int Brush::appendVertex(const QVector3D &v)
{
    m_Vertices.append(v);
    return m_Vertices.count() - 1;
}

int Brush::vertexCount() const
{
    return m_Vertices.count();
}

void Brush::removeVertex(int index)
{
    Q_ASSERT(index >= 0 && index < m_Vertices.count());

    removeVertexFromChildFaces(index);
    m_Vertices.removeAt(index);
}

void Brush::clearVertices()
{
    clearVerticesFromChildFaces();
    m_Vertices.clear();
}

QList<QVector3D> Brush::vertexList() const
{
    return m_Vertices;
}

QList<BrushFace*> Brush::faces() const
{
    return findChildren<BrushFace*>(QString(), Qt::FindDirectChildrenOnly);
}

void Brush::removeVertexFromChildFaces(int index)
{
    QList<BrushFace*> childFaces = faces();
    foreach ( BrushFace* face, childFaces )
    {
        face->notifyVertexRemoved(index);
    }
}

void Brush::clearVerticesFromChildFaces()
{
    QList<BrushFace*> childFaces = faces();
    foreach ( BrushFace* face, childFaces )
    {
        face->clearVertices();
    }
}

QList<QVector3D> Brush::vertexList(const QList<int> &indices) const
{
    QList<QVector3D> verts;

    foreach ( int i, indices )
    {
        verts.append(m_Vertices.at(i));
    }

    return verts;
}

void Brush::replaceVertex(int index, const QVector3D &v)
{
    m_Vertices.replace(index, v);
    updateVertexInChildFaces(index);
}

void Brush::updateVertexInChildFaces(int index)
{
    QList<BrushFace*> childFaces = faces();
    foreach ( BrushFace* face, childFaces )
    {
        face->notifyVertexChanged(index);
    }
}
