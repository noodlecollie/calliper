#include "brush.h"
#include "brushface.h"
#include "jsonutil.h"

Brush::Brush(SceneObject *parent) : SceneObject(parent)
{
    initDefaults();
}

Brush::~Brush()
{
}

void Brush::initDefaults()
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

void Brush::appendVertices(const QList<QVector3D> &verts)
{
    m_Vertices.append(verts);
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

Brush::Brush(const Brush &cloneFrom) : SceneObject(cloneFrom), m_Vertices(cloneFrom.m_Vertices)
{

}

SceneObject* Brush::clone() const
{
    return new Brush(*this);
}

QString Brush::serialiseIdentifier() const
{
    return staticMetaObject.className();
}

bool Brush::serialiseToJson(QJsonObject &obj) const
{
    obj.insert(ISerialisable::KEY_IDENTIFIER(), QJsonValue(Brush::serialiseIdentifier()));

    // Serialise the parent object first.
    QJsonObject jsonParent;
    SceneObject::serialiseToJson(jsonParent);

    // Insert this as the superclass.
    obj.insert(ISerialisable::KEY_SUPERCLASS(), QJsonValue(jsonParent));

    // Serialise vertices.
    QJsonArray arrVertices;
    foreach ( QVector3D vec, m_Vertices )
    {
        QJsonArray arrVec;
        JsonUtil::vector3ToJsonArray<QVector3D>(vec, arrVec);
        arrVertices.append(QJsonValue(arrVec));
    }

    obj.insert("vertices", QJsonValue(arrVertices));

    return true;
}

Brush::Brush(const QJsonObject &serialisedData, SceneObject *parent) :
    SceneObject(serialisedData.value(ISerialisable::KEY_SUPERCLASS()).toObject(), parent)
{
    initDefaults();

    if ( !validateIdentifier(serialisedData, Brush::serialiseIdentifier()) )
        return;

    // TODO: Read
}
