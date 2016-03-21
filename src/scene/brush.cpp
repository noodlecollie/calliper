#include "brush.h"
#include "brushface.h"
#include "jsonutil.h"
#include "shaderstack.h"

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

void Brush::appendVertices(const QVector<QVector3D> &verts)
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

QVector<QVector3D> Brush::vertexList() const
{
    return m_Vertices;
}

QVector<BrushFace*> Brush::faces() const
{
    return findChildren<BrushFace*>(QString(), Qt::FindDirectChildrenOnly).toVector();
}

void Brush::removeVertexFromChildFaces(int index)
{
    QVector<BrushFace*> childFaces = faces();
    foreach ( BrushFace* face, childFaces )
    {
        face->notifyVertexRemoved(index);
    }
}

void Brush::clearVerticesFromChildFaces()
{
    QVector<BrushFace*> childFaces = faces();
    foreach ( BrushFace* face, childFaces )
    {
        face->clearVertices();
    }
}

QVector<QVector3D> Brush::vertexList(const QVector<int> &indices) const
{
    QVector<QVector3D> verts;

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
    QVector<BrushFace*> childFaces = faces();
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

    QJsonArray arrFaces;
    QVector<BrushFace*> faceList = faces();
    foreach ( BrushFace* f, faceList )
    {
        QJsonObject o;
        f->serialiseToJson(o);
        arrFaces.append(o);
    }

    obj.insert("faces", QJsonValue(arrFaces));

    return true;
}

Brush::Brush(const QJsonObject &serialisedData, SceneObject *parent) :
    SceneObject(serialisedData.value(ISerialisable::KEY_SUPERCLASS()).toObject(), parent)
{
    initDefaults();

    if ( !validateIdentifier(serialisedData, Brush::serialiseIdentifier()) )
        return;

    QJsonValue vVertices = serialisedData.value("vertices");
    QJsonArray arrVertices = vVertices.toArray();
    if ( !vVertices.isArray() || arrVertices.count() < 1 )
    {
        qWarning() << "Vertices array within brush" << objectName() << "is invalid!";
        return;
    }

    for ( int i = 0; i < arrVertices.count(); i++ )
    {
        QJsonValue vVertex = arrVertices.at(i);
        QJsonArray arrVertex = vVertex.toArray();
        if ( !vVertex.isArray() || arrVertex.count() < 3 )
        {
            // Fail first - better this way, as it's clear that a brush is
            // invalid if it has no vertices. This should make it easier to
            // detect when unserialising faces.
            qWarning() << "Vertex" << i << "in brush" << objectName() << "is invalid!";
            m_Vertices.clear();
            return;
        }

        m_Vertices.append(JsonUtil::jsonArrayToVector3<QVector3D>(arrVertex));
    }

    QJsonValue vFaces = serialisedData.value("faces");
    if ( vFaces.isArray() )
    {
        QJsonArray arrFaces = vFaces.toArray();
        for ( int i = 0; i < arrFaces.count(); i++ )
        {
            QJsonValue f = arrFaces.at(i);
            if ( f.isObject() )
                new BrushFace(this, f.toObject());
        }
    }
}

void Brush::draw(ShaderStack *stack)
{
    QVector<BrushFace*> faceList = faces();
    foreach ( BrushFace* f , faceList )
    {
        f->draw(stack);
    }
}
