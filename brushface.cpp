#include "brushface.h"
#include "brush.h"

BrushFace::BrushFace(Brush *parent, QList<int> vertices) : SceneObject(parent),
    m_Vertices(vertices)
{
    Q_ASSERT(parent);
}

BrushFace::~BrushFace()
{
}

Brush* BrushFace::parentBrush() const
{
    return qobject_cast<Brush*>(parent());
}

QString BrushFace::texturePath() const
{
    return m_szTexturePath;
}

void BrushFace::setTexturePath(const QString &path)
{
    m_szTexturePath = path;
}

int BrushFace::vertexAt(int index) const
{
    return m_Vertices.at(index);
}

void BrushFace::appendVertex(int v)
{
    m_Vertices.append(v);
}

int BrushFace::vertexCount() const
{
    return m_Vertices.count();
}

void BrushFace::removeVertex(int index)
{
    m_Vertices.removeAt(index);
}

QList<int> BrushFace::vertexList() const
{
    return m_Vertices;
}

void BrushFace::clearVertices()
{
    m_Vertices.clear();
}

void BrushFace::notifyVertexRemoved(int index)
{
    // A vertex at the given index was removed.
    // Firstly remove it if we have a reference to it.
    m_Vertices.removeAll(index);

    // Any vertices whose index is higher than this will
    // now have been shifted one index down.
    for ( int i = 0; i < m_Vertices.count(); i++ )
    {
        int& v = m_Vertices[i];

        if ( v > index )
            v--;
    }
}

QList<QVector3D> BrushFace::referencedVertexList() const
{
    return parentBrush()->vertexList(m_Vertices);
}
