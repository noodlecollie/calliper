#include "cvertexbundle.h"
#include <QVector2D>

CVertexBundle::CVertexBundle(QObject *parent) : QObject(parent),
    m_VertexBuffer(QOpenGLBuffer::VertexBuffer),
    m_IndexBuffer(QOpenGLBuffer::IndexBuffer)
{
    m_iInterleavingFormat = COpenGLRenderer::FormatPosition;
    m_bVBufferDirty = true;
    m_bIBufferDirty = true;
}

CVertexBundle::~CVertexBundle()
{
    if ( m_VertexBuffer.isCreated() )
    {
        m_VertexBuffer.destroy();
    }

    if ( m_IndexBuffer.isCreated() )
    {
        m_IndexBuffer.destroy();
    }
}

COpenGLRenderer::InterleavingFormat CVertexBundle::interleavingFormat() const
{
    return m_iInterleavingFormat;
}

void CVertexBundle::setInterleavingFormat(COpenGLRenderer::InterleavingFormat format)
{
    if ( format == m_iInterleavingFormat ) return;

    m_iInterleavingFormat = format;
    m_bVBufferDirty = true;
}

void CVertexBundle::clear()
{
    m_VertexData.clear();
    m_IndexData.clear();
    m_bVBufferDirty = true;
    m_bIBufferDirty = true;
}

void CVertexBundle::clearIndices()
{
    m_IndexData.clear();
    m_bIBufferDirty = true;
}

void CVertexBundle::clearVertices()
{
    m_VertexData.clear();
    m_bVBufferDirty = true;
}

int CVertexBundle::vertexCount() const
{
    return m_VertexData.count() / floatsPerVertex();
}

int CVertexBundle::indexCount() const
{
    return m_IndexData.count();
}

int CVertexBundle::floatsPerVertex() const
{
    return COpenGLRenderer::interleavingFormatSize(m_iInterleavingFormat) / sizeof(float);
}

void CVertexBundle::appendVertex(const QVector3D &position)
{
    int vertex = incrementSize();
    setVertexPosition(vertex, position);
}

void CVertexBundle::appendVertex(const QVector3D &position, const QVector2D &uv)
{
    int vertex = incrementSize();
    setVertexPosition(vertex, position);
    setVertexUV(vertex, uv);
}

int CVertexBundle::incrementSize()
{
    int numVertices = vertexCount();
    m_VertexData.resize((numVertices+1) * floatsPerVertex());
    return numVertices;
}

void CVertexBundle::setVertexPosition(int vertex, const QVector3D &position)
{
    int startIndex = vertex * floatsPerVertex();
    m_VertexData[startIndex] = position.x();
    m_VertexData[startIndex+1] = position.y();
    m_VertexData[startIndex+2] = position.z();
    m_bVBufferDirty = true;
}

void CVertexBundle::setVertexUV(int vertex, const QVector2D &uv)
{
    int startIndex = vertex * floatsPerVertex();

    startIndex += (COpenGLRenderer::attributeOffset(m_iInterleavingFormat, COpenGLRenderer::UV) / sizeof(float));

    m_VertexData[startIndex] = uv.x();
    m_VertexData[startIndex+1] = uv.y();
    m_bVBufferDirty = true;
}

//float* CVertexBundle::vertexData()
//{
//    return m_VertexData.data();
//}

const float* CVertexBundle::vertexConstData() const
{
    return m_VertexData.constData();
}

//unsigned int* CVertexBundle::indexData()
//{
//    return m_IndexData.data();
//}

const unsigned int* CVertexBundle::indexConstData() const
{
    return m_IndexData.constData();
}

void CVertexBundle::appendIndex(unsigned int idx)
{
    m_IndexData.append(idx);
    m_bIBufferDirty = true;
}

void CVertexBundle::setIndex(int position, unsigned int value)
{
    m_IndexData[position] = value;
    m_bIBufferDirty = true;
}

unsigned int CVertexBundle::vertexDataSize() const
{
    return m_VertexData.count() * sizeof(float);
}

unsigned int CVertexBundle::indexDataSize() const
{
    return m_IndexData.count() * sizeof(unsigned int);
}

bool CVertexBundle::isVertexDataDirty() const
{
    return m_bVBufferDirty;
}

bool CVertexBundle::isIndexDataDirty() const
{
    return m_bIBufferDirty;
}

void CVertexBundle::createBuffer(QOpenGLBuffer &buffer)
{
    if ( !buffer.isCreated() )
    {
        bool success = buffer.create();
        Q_ASSERT(success);
    }
}

void CVertexBundle::uploadData(QOpenGLBuffer &buffer, const void *data, int size)
{
    bool success = buffer.bind();
    Q_ASSERT(success);

    buffer.allocate(data, size);

    buffer.release();
}

void CVertexBundle::upload(bool force)
{
    createBuffer(m_VertexBuffer);
    if ( force || m_bVBufferDirty )
        uploadData(m_VertexBuffer, vertexConstData(), vertexDataSize());
    m_bVBufferDirty = false;

    createBuffer(m_IndexBuffer);
    if ( force || m_bIBufferDirty )
        uploadData(m_IndexBuffer, indexConstData(), indexDataSize());
    m_bIBufferDirty = false;
}

bool CVertexBundle::bindVertexBuffer(bool bind)
{
    if ( bind )
    {
        return m_VertexBuffer.bind();
    }
    else
    {
        m_VertexBuffer.release();
        return true;
    }
}

bool CVertexBundle::bindIndexBuffer(bool bind)
{
    if ( bind )
    {
        return m_IndexBuffer.bind();
    }
    else
    {
        m_IndexBuffer.release();
        return true;
    }
}
