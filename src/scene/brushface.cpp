#include "brushface.h"
#include "brush.h"
#include "shaderstack.h"
#include "mapdocument.h"
#include "mapscene.h"
#include "resourcemanager.h"
#include "textureplane.h"

BrushFace::BrushFace(Brush *parent, QList<int> vertices) : SceneObject(parent),
    m_Vertices(vertices)
{
    Q_ASSERT(parent);
    m_bRebuildGeometry = true;

    m_pTexturePlane = new TexturePlane(this);
    connect(m_pTexturePlane, &TexturePlane::dataChanged, this, &BrushFace::forceRebuildGeometry);
}

BrushFace::~BrushFace()
{
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

QList<int> BrushFace::vertexList() const
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

QList<QVector3D> BrushFace::referencedVertexList() const
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

    QList<QVector3D> verts = referencedVertexList();
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

    qDebug() << "Scale" << m_pTexturePlane->scale() << "gives point" << QVector3D(1,0,0)
             << "as texture co-ordinate" << m_pTexturePlane->textureCoordinate(QVector3D(1,0,0), texSize, QVector3D(0,-1,0));
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

    stack->modelToWorldPostMultiply(localToParent());

    if ( geometry()->isEmpty() )
        return;

    // Upload and bind the geometry.
    geometry()->upload();
    geometry()->bindVertices(true);
    geometry()->bindIndices(true);

    // Apply the data format.
    geometry()->applyDataFormat(stack->shaderTop());

    // If we're selected, set the global colour.
    bool pushedColor = false;
    MapDocument* doc = m_pScene->document();
    if ( doc->selectedSet().contains(this) )
    {
        pushedColor = true;
        stack->globalColorPush();
        stack->globalColorSetTop(doc->selectedColor());
    }

    // Apply the texture.
    QOpenGLTexture* tex = resourceManager()->texture(geometry()->texture(0));
    tex->bind(0);

    // Draw.
    geometry()->draw();

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
