#include "cvertexbundle.h"
#include <QVector2D>

static const int ATTRIBUTE_SIZE[] = {
    3 * sizeof(float),      // Position
    2 * sizeof(float),      // UV
};

static const int FORMAT_SIZE[] = {
    ATTRIBUTE_SIZE[CVertexBundle::Position],
    ATTRIBUTE_SIZE[CVertexBundle::Position] + ATTRIBUTE_SIZE[CVertexBundle::UV],
};

int CVertexBundle::attributeOffset(InterleavingFormat format, Attribute att)
{
    switch (format)
    {
        case FormatPosition:
        {
            switch (att)
            {
            case Position:
                return 0;
            default:
                return 0;
            }
        }

        case FormatPositionUV:
        {
            switch(att)
            {
            case Position:
                return 0;
            case UV:
                return 3 * sizeof(float);
            default:
                return 0;
            }
        }

    default:
        return 0;
    }
}

int CVertexBundle::attributeSize(Attribute att)
{
    return ATTRIBUTE_SIZE[att];
}

int CVertexBundle::interleavingFormatSize(InterleavingFormat format)
{
    return FORMAT_SIZE[format];
}

CVertexBundle::CVertexBundle(QObject *parent) : QObject(parent)
{
    m_iInterleavingFormat = FormatPosition;
}

CVertexBundle::InterleavingFormat CVertexBundle::interleavingFormat() const
{
    return m_iInterleavingFormat;
}

void CVertexBundle::setInterleavingFormat(InterleavingFormat format)
{
    if ( format == m_iInterleavingFormat ) return;

    m_iInterleavingFormat = format;
}

void CVertexBundle::clear()
{
    m_VertexData.clear();
    m_IndexData.clear();
}

void CVertexBundle::clearIndices()
{
    m_IndexData.clear();
}

void CVertexBundle::clearVertices()
{
    m_VertexData.clear();
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
    return FORMAT_SIZE[m_iInterleavingFormat] / sizeof(float);
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
}

void CVertexBundle::setVertexUV(int vertex, const QVector2D &uv)
{
    int startIndex = vertex * floatsPerVertex();

    startIndex += (attributeOffset(m_iInterleavingFormat, UV) / sizeof(float));

    m_VertexData[startIndex] = uv.x();
    m_VertexData[startIndex+1] = uv.y();
}

float* CVertexBundle::vertexData()
{
    return m_VertexData.data();
}

const float* CVertexBundle::vertexConstData() const
{
    return m_VertexData.constData();
}

unsigned int* CVertexBundle::indexData()
{
    return m_IndexData.data();
}

const unsigned int* CVertexBundle::indexConstData() const
{
    return m_IndexData.constData();
}

void CVertexBundle::appendIndex(unsigned int idx)
{
    m_IndexData.append(idx);
}

void CVertexBundle::setIndex(int position, unsigned int value)
{
    m_IndexData[position] = value;
}

unsigned int CVertexBundle::vertexDataSize() const
{
    return m_VertexData.count() * sizeof(float);
}

unsigned int CVertexBundle::indexDataSize() const
{
    return m_IndexData.count() * sizeof(unsigned int);
}
