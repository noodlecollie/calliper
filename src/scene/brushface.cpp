#include "brushface.h"
#include "brush.h"
#include "shaderstack.h"
#include "mapdocument.h"
#include "mapscene.h"
#include "resourcemanager.h"
#include "textureplane.h"
#include "jsonutil.h"

BrushFace::BrushFace(Brush *parent, QVector<int> vertices) : QObject(parent),
    m_Vertices(vertices)
{
    Q_ASSERT(parent);
    initDefaults();
}

BrushFace::BrushFace(Brush *parent, const QJsonObject &serialisedData) : QObject(parent)
{
    Q_ASSERT(parent);
    initDefaults();

    if ( !validateIdentifier(serialisedData, BrushFace::serialiseIdentifier()) )
        return;

    QJsonValue vVertices;
    if ( vVertices.isArray() )
    {
        m_Vertices = JsonUtil::integerJsonArrayToVector<int>(vVertices.toArray());
    }

    QJsonValue vTexturePlane;
    if ( vTexturePlane.isObject() )
    {
        delete m_pTexturePlane;
        m_pTexturePlane = new TexturePlane(vTexturePlane.toObject(), this);
        connect(m_pTexturePlane, &TexturePlane::dataChanged, this, &BrushFace::forceRebuildGeometry);
    }
}

void BrushFace::initDefaults()
{
    m_pGeometry = new GeometryData();
    m_bRebuildGeometry = true;

    m_pTexturePlane = new TexturePlane(this);
    connect(m_pTexturePlane, &TexturePlane::dataChanged, this, &BrushFace::forceRebuildGeometry);
}

BrushFace::~BrushFace()
{
    delete m_pGeometry;
}

Brush* BrushFace::parentBrush() const
{
    return qobject_cast<Brush*>(parent());
}

int BrushFace::vertexAt(int index) const
{
    return m_Vertices.at(index);
}

void BrushFace::appendVertex(int v)
{
    m_Vertices.append(v);
    m_bRebuildGeometry = true;
}

int BrushFace::vertexCount() const
{
    return m_Vertices.count();
}

void BrushFace::removeVertex(int index)
{
    m_Vertices.removeAt(index);
    m_bRebuildGeometry = true;
}

QVector<int> BrushFace::vertexList() const
{
    return m_Vertices;
}

void BrushFace::clearVertices()
{
    m_Vertices.clear();
    m_bRebuildGeometry = true;
}

void BrushFace::notifyVertexRemoved(int index)
{
    // A vertex at the given index was removed.
    // Firstly remove it if we have a reference to it.
    if ( m_Vertices.removeAll(index) > 0 )
    {
        m_bRebuildGeometry = true;

        // Any vertices whose index is higher than this will
        // now have been shifted one index down.
        for ( int i = 0; i < m_Vertices.count(); i++ )
        {
            int& v = m_Vertices[i];

            if ( v > index )
                v--;
        }
    }
}

QVector<QVector3D> BrushFace::referencedVertexList() const
{
    return parentBrush()->vertexList(m_Vertices);
}

bool BrushFace::isValid() const
{
    return m_Vertices.count() >= 3;
}

QVector3D BrushFace::normal() const
{
    if ( !isValid() )
        return QVector3D();

    Brush* b = parentBrush();
    return QVector3D::normal(b->vertexAt(m_Vertices.at(0)), b->vertexAt(m_Vertices.at(1)));
}

void BrushFace::buildGeometry()
{
    m_pGeometry->clear();

    QVector<QVector3D> verts = referencedVertexList();
    if ( verts.count() < 3 )
        return;

    QVector3D normal = QVector3D::normal(verts.at(0), verts.at(1), verts.at(2));

    // Add all vertices.
    // UVs are calculated later.
    QOpenGLTexture* tex = resourceManager()->texture(m_pTexturePlane->texturePath());
    QSize texSize(tex->width(), tex->height());
    foreach ( QVector3D v, verts )
    {
        m_pGeometry->appendVertex(v, normal, m_pTexturePlane->textureCoordinate(v, texSize, normal));
    }

    // Add all indices.
    for ( int i = 1; i < verts.count() - 1; i++ )
    {
        m_pGeometry->appendIndexTriangle(0, i, i+1);
    }

    m_pGeometry->setTexture(0, m_pTexturePlane->texturePath());

    m_bRebuildGeometry = false;
}

void BrushFace::notifyVertexChanged(int index)
{
    if ( m_Vertices.contains(index) )
        m_bRebuildGeometry = true;
}

void BrushFace::draw(ShaderStack *stack)
{
    if ( m_bRebuildGeometry )
        buildGeometry();

    if ( m_pGeometry->isEmpty() )
        return;

    // Upload and bind the geometry.
    m_pGeometry->upload();
    m_pGeometry->bindVertices(true);
    m_pGeometry->bindIndices(true);

    // Apply the data format.
    m_pGeometry->applyDataFormat(stack->shaderTop());

    // If we're selected, set the global colour.
    bool pushedColor = false;
    Brush* b = parentBrush();
    MapDocument* doc = b->scene()->document();
    if ( doc->selectedSet().contains(b) )
    {
        pushedColor = true;
        stack->globalColorPush();
        stack->globalColorSetTop(doc->selectedColor());
    }

    // Apply the texture.
    QOpenGLTexture* tex = resourceManager()->texture(m_pGeometry->texture(0));
    tex->bind(0);

    // Draw.
    m_pGeometry->draw();

    if ( pushedColor )
    {
        stack->globalColorPop();
    }
}

void BrushFace::forceRebuildGeometry()
{
    m_bRebuildGeometry = true;
}

TexturePlane* BrushFace::texturePlane() const
{
    return m_pTexturePlane;
}

bool BrushFace::serialiseToJson(QJsonObject &obj) const
{
    obj.insert(ISerialisable::KEY_IDENTIFIER(), QJsonValue(BrushFace::serialiseIdentifier()));

    QJsonArray arrVertices;
    JsonUtil::vectorToJsonArray<int,int>(m_Vertices, arrVertices);
    obj.insert("vertices", QJsonValue(arrVertices));

    QJsonObject planeObj;
    m_pTexturePlane->serialiseToJson(planeObj);
    obj.insert("texturePlane", QJsonValue(planeObj));

    return true;
}

QString BrushFace::serialiseIdentifier() const
{
    return staticMetaObject.className();
}

float BrushFace::computeIntersection(const Ray3D &ray, QRgb *col) const
{
    QVector3D nrm = normal();
    if (nrm.isNull())
        return (float)qInf();

    // We assume that the normal and the ray direction are both unit vectors.
    // If they're perpendicular, we're parallel to the plane.
    if ( QVector3D::dotProduct(nrm, ray.direction()) == 0 )
        return (float)qInf();

    // TODO
}

float BrushFace::planeDistanceFromOrigin() const
{
    if ( !isValid() || !parentBrush() )
        return 0;

    return QVector3D::dotProduct(normal(), parentBrush()->vertexAt(0));
}
