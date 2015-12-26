#include "geometrydata.h"
#include <QOpenGLBuffer>
#include <QOpenGLFunctions_4_1_Core>
#include "resourcemanager.h"

#define VERTEX_ELEMENT_COUNT (3+3+2)

GeometryData::GeometryData()
{
    m_bVerticesStale = true;
    m_bIndicesStale = true;

    m_pVertexBuffer = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    m_pIndexBuffer = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);

    m_iDrawMode = GL_TRIANGLES;
}

GeometryData::~GeometryData()
{
    destroy();
    delete m_pVertexBuffer;
    delete m_pIndexBuffer;
}

void GeometryData::appendVertex(const QVector3D &pos, const QVector3D &normal, const QVector2D &uv)
{
    int size = m_Vertices.size();
    m_Vertices.resize(size + 8);

    m_Vertices[size+0] = pos.x();
    m_Vertices[size+1] = pos.y();
    m_Vertices[size+2] = pos.z();

    m_Vertices[size+3] = normal.x();
    m_Vertices[size+4] = normal.y();
    m_Vertices[size+5] = normal.z();

    m_Vertices[size+6] = uv.x();
    m_Vertices[size+7] = uv.y();

    m_bVerticesStale = true;
}

void GeometryData::appendVertex(const QVector3D &pos, const QColor &col)
{
    int size = m_Vertices.size();
    m_Vertices.resize(size + 7);

    m_Vertices[size+0] = pos.x();
    m_Vertices[size+1] = pos.y();
    m_Vertices[size+2] = pos.z();

    m_Vertices[size+3] = col.redF();
    m_Vertices[size+4] = col.greenF();
    m_Vertices[size+5] = col.blueF();
    m_Vertices[size+6] = col.alphaF();

    m_bVerticesStale = true;
}

int GeometryData::vertexCount() const
{
    return m_Vertices.count() / VERTEX_ELEMENT_COUNT;
}

int GeometryData::vertexBytes() const
{
    return m_Vertices.count() * sizeof(float);
}

void GeometryData::appendIndex(unsigned int i)
{
    m_Indices.append(i);

    m_bIndicesStale = true;
}

int GeometryData::indexCount() const
{
    return m_Indices.count();
}

int GeometryData::indexBytes() const
{
    return m_Indices.count() * sizeof(float);
}

void GeometryData::upload(bool force)
{
    if ( isEmpty() && !force )
        return;

    if ( !m_pVertexBuffer->isCreated() )
    {
        bool success = m_pVertexBuffer->create();
        Q_ASSERT(success);
    }

    if ( !m_pIndexBuffer->isCreated() )
    {
        bool success = m_pIndexBuffer->create();
        Q_ASSERT(success);
    }

    if ( m_bVerticesStale || force )
    {
        bool success = m_pVertexBuffer->bind();
        Q_ASSERT(success);
        m_pVertexBuffer->allocate(m_Vertices.constData(), vertexBytes());
        m_bVerticesStale = false;
    }

    if ( m_bIndicesStale || force )
    {
        bool success = m_pIndexBuffer->bind();
        Q_ASSERT(success);
        m_pIndexBuffer->allocate(m_Indices.constData(), indexBytes());
        m_bIndicesStale = false;
    }
}

void GeometryData::clearVertices()
{
    m_Vertices.clear();
    m_bVerticesStale = true;
}

void GeometryData::clearIndices()
{
    m_Indices.clear();
    m_bIndicesStale = true;
}

void GeometryData::bindVertices(bool bind)
{
    if ( bind )
    {
        bool success = m_pVertexBuffer->bind();
        Q_ASSERT(success);
    }
    else
    {
        m_pVertexBuffer->release();
    }
}

void GeometryData::bindIndices(bool bind)
{
    if ( bind )
    {
        bool success = m_pIndexBuffer->bind();
        Q_ASSERT(success);
    }
    else
    {
        m_pIndexBuffer->release();
    }
}

void GeometryData::draw()
{
    QOpenGLFunctions_4_1_Core* f = resourceManager()->functions();

    f->glDrawElements(m_iDrawMode, indexCount(), GL_UNSIGNED_INT, 0);
}

QString GeometryData::texture(int index) const
{
    if ( index < 0 || index >= MAX_GEOM_TEXTURES ) return QString();
    return m_Textures[index];
}

void GeometryData::setTexture(int index, const QString &path)
{
    if ( index < 0 || index >= MAX_GEOM_TEXTURES ) return;
    m_Textures[index] = path;
}

void GeometryData::clear()
{
    clearVertices();
    clearIndices();
}

bool GeometryData::isEmpty() const
{
    return vertexCount() < 1 && indexCount() < 1;
}

void GeometryData::destroy()
{
    if ( m_pVertexBuffer->isCreated() )
        m_pVertexBuffer->destroy();

    if ( m_pIndexBuffer->isCreated() )
        m_pIndexBuffer->destroy();
}

GLenum GeometryData::drawMode() const
{
    return m_iDrawMode;
}

void GeometryData::setDrawMode(GLenum mode)
{
    m_iDrawMode = mode;
}
