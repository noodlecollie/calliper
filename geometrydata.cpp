#include "geometrydata.h"
#include <QOpenGLBuffer>
#include <QOpenGLFunctions_4_1_Core>
#include "resourcemanager.h"
#include "shaderprogram.h"
#include <QtDebug>
#include <QOpenGLTexture>

QDebug operator<<(QDebug debug, const GeometryData &data)
{
    debug.nospace() << "GeometryData(V: " << data.vertexCount() << " I: " << data.indexCount() << ")";
    return debug;
}

static int numComponents[] = {
    3,  // Position
    3,  // Normal
    4,  // Color
    2,  // UV
};

static const int formatStride[] = {
    8*sizeof(float),    // PositionNormalUV
    10*sizeof(float),    // PositionNormalColor
    12*sizeof(float),   // PositionNormalUVColor
};

static int formatOffset(GeometryData::DataFormat format, ShaderProgram::Attribute att)
{
    switch (format)
    {
        case GeometryData::PositionNormalUV:
        {
            switch (att)
            {
            case ShaderProgram::Position:
                return 0;

            case ShaderProgram::Normal:
                return 3*sizeof(float);

            case ShaderProgram::UV:
                return 6*sizeof(float);

            default:
                Q_ASSERT(false);
                return 0;
            }
        }

        case GeometryData::PositionNormalColor:
        {
            switch (att)
            {
            case ShaderProgram::Position:
                return 0;

            case ShaderProgram::Normal:
                return 3*sizeof(float);

            case ShaderProgram::Color:
                return 6*sizeof(float);

            default:
                Q_ASSERT(false);
                return 0;
            }
        }

        case GeometryData::PositionNormalUVColor:
        {
            switch (att)
            {
            case ShaderProgram::Position:
                return 0;

            case ShaderProgram::Normal:
                return 3*sizeof(float);

            case ShaderProgram::UV:
                return 6*sizeof(float);

            case ShaderProgram::Color:
                return 8*sizeof(float);

            default:
                Q_ASSERT(false);
                return 0;
            }
        }

        default:
        {
            Q_ASSERT(false);
            return 0;
        }
    }
}

int GeometryData::vertexFormatBytes() const
{
    return formatStride[m_iDataFormat];
}

GeometryData::GeometryData()
{
    m_bVerticesStale = true;
    m_bIndicesStale = true;

    m_pVertexBuffer = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    m_pIndexBuffer = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);

    m_iDrawMode = GL_TRIANGLES;
    m_flLineWidth = 1.0f;

    m_iDataFormat = PositionNormalUV;
}

GeometryData::GeometryData(const GeometryData &other)
{
    m_bVerticesStale = true;
    m_bIndicesStale = true;

    m_pVertexBuffer = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    m_pIndexBuffer = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);

    m_Vertices = other.m_Vertices;
    m_Indices = other.m_Indices;

    for ( int i = 0; i < MAX_GEOM_TEXTURES; i++ )
    {
        m_Textures[i] = other.m_Textures[i];
    }

    m_iDrawMode = other.m_iDrawMode;
    m_flLineWidth = other.m_flLineWidth;
    m_iDataFormat = other.m_iDataFormat;
    m_pLocalTexture = other.m_pLocalTexture;
    m_szShaderOverride = other.m_szShaderOverride;
}

GeometryData::~GeometryData()
{
    destroy();
    delete m_pVertexBuffer;
    delete m_pIndexBuffer;
}

void GeometryData::appendVertex(const QVector3D &pos, const QVector3D &normal, const QVector2D &uv)
{
    m_iDataFormat = PositionNormalUV;
    int size = m_Vertices.size();
    int increment = formatStride[m_iDataFormat]/sizeof(float);
    m_Vertices.resize(size + increment);

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

void GeometryData::appendVertex(const QVector3D &pos, const QVector3D &normal, const QColor &col)
{
    m_iDataFormat = PositionNormalColor;
    int size = m_Vertices.size();
    int increment = formatStride[m_iDataFormat]/sizeof(float);
    m_Vertices.resize(size + increment);

    m_Vertices[size+0] = pos.x();
    m_Vertices[size+1] = pos.y();
    m_Vertices[size+2] = pos.z();

    m_Vertices[size+3] = normal.x();
    m_Vertices[size+4] = normal.y();
    m_Vertices[size+5] = normal.z();

    m_Vertices[size+6] = col.redF();
    m_Vertices[size+7] = col.greenF();
    m_Vertices[size+8] = col.blueF();
    m_Vertices[size+9] = col.alphaF();

    m_bVerticesStale = true;
}

void GeometryData::appendVertex(const QVector3D &pos, const QVector3D &normal, const QVector2D &uv, const QColor &col)
{
    m_iDataFormat = PositionNormalUVColor;
    int size = m_Vertices.size();
    int increment = formatStride[m_iDataFormat]/sizeof(float);
    m_Vertices.resize(size + increment);

    m_Vertices[size+0] = pos.x();
    m_Vertices[size+1] = pos.y();
    m_Vertices[size+2] = pos.z();

    m_Vertices[size+3] = normal.x();
    m_Vertices[size+4] = normal.y();
    m_Vertices[size+5] = normal.z();

    m_Vertices[size+6] = uv.x();
    m_Vertices[size+7] = uv.y();

    m_Vertices[size+8] = col.redF();
    m_Vertices[size+9] = col.greenF();
    m_Vertices[size+10] = col.blueF();
    m_Vertices[size+11] = col.alphaF();

    m_bVerticesStale = true;
}

int GeometryData::vertexCount() const
{
    return m_Vertices.count() / (formatStride[m_iDataFormat]/sizeof(float));
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

void GeometryData::draw(int offset, int count)
{
    QOpenGLFunctions_4_1_Core* f = resourceManager()->functions();

    f->glLineWidth(m_flLineWidth);
    f->glDrawElements(m_iDrawMode, count < 0 ? indexCount() : count, GL_UNSIGNED_INT, reinterpret_cast<void*>(offset));
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

GeometryData::DataFormat GeometryData::dataFormat() const
{
    return m_iDataFormat;
}

void GeometryData::setDataFormat(DataFormat format)
{
    m_iDataFormat = format;
}

void GeometryData::applyDataFormat(ShaderProgram *program)
{
    switch (m_iDataFormat)
    {
        case PositionNormalUV:
        {
            program->setAttributeFormat(
                        ShaderProgram::Position,
                        numComponents[ShaderProgram::Position],
                        formatStride[m_iDataFormat],
                        formatOffset(m_iDataFormat, ShaderProgram::Position));

            program->setAttributeFormat(
                        ShaderProgram::Normal,
                        numComponents[ShaderProgram::Normal],
                        formatStride[m_iDataFormat],
                        formatOffset(m_iDataFormat, ShaderProgram::Normal));

            program->setAttributeFormat(
                        ShaderProgram::UV,
                        numComponents[ShaderProgram::UV],
                        formatStride[m_iDataFormat],
                        formatOffset(m_iDataFormat, ShaderProgram::UV));

            break;
        }

        case PositionNormalColor:
        {
            program->setAttributeFormat(
                        ShaderProgram::Position,
                        numComponents[ShaderProgram::Position],
                        formatStride[m_iDataFormat],
                        formatOffset(m_iDataFormat, ShaderProgram::Position));

            program->setAttributeFormat(
                        ShaderProgram::Normal,
                        numComponents[ShaderProgram::Normal],
                        formatStride[m_iDataFormat],
                        formatOffset(m_iDataFormat, ShaderProgram::Normal));

            program->setAttributeFormat(
                        ShaderProgram::Color,
                        numComponents[ShaderProgram::Color],
                        formatStride[m_iDataFormat],
                        formatOffset(m_iDataFormat, ShaderProgram::Color));

            break;
        }

        case PositionNormalUVColor:
        {
            program->setAttributeFormat(
                        ShaderProgram::Position,
                        numComponents[ShaderProgram::Position],
                        formatStride[m_iDataFormat],
                        formatOffset(m_iDataFormat, ShaderProgram::Position));

            program->setAttributeFormat(
                        ShaderProgram::Normal,
                        numComponents[ShaderProgram::Normal],
                        formatStride[m_iDataFormat],
                        formatOffset(m_iDataFormat, ShaderProgram::Normal));

            program->setAttributeFormat(
                        ShaderProgram::UV,
                        numComponents[ShaderProgram::UV],
                        formatStride[m_iDataFormat],
                        formatOffset(m_iDataFormat, ShaderProgram::UV));

            program->setAttributeFormat(
                        ShaderProgram::Color,
                        numComponents[ShaderProgram::Color],
                        formatStride[m_iDataFormat],
                        formatOffset(m_iDataFormat, ShaderProgram::Color));

            break;
        }

        default:
        {
            Q_ASSERT(false);
            break;
        }
    }
}

QSharedPointer<QOpenGLTexture> GeometryData::localTexture() const
{
    return m_pLocalTexture;
}

void GeometryData::setLocalTexture(const QSharedPointer<QOpenGLTexture> &tex)
{
    m_pLocalTexture = tex;
}

bool GeometryData::hasLocalTexture() const
{
    return !m_pLocalTexture.isNull();
}

QString GeometryData::shaderOverride() const
{
    return m_szShaderOverride;
}

void GeometryData::setShaderOverride(const QString &shader)
{
    m_szShaderOverride = shader;
}

float GeometryData::lineWidth() const
{
    return m_flLineWidth;
}

void GeometryData::setLineWidth(float width)
{
    m_flLineWidth = width;
}

float* GeometryData::vertexAt(int i)
{
    return m_Vertices.data() + (i * (formatStride[m_iDataFormat]/sizeof(float)));
}

unsigned int* GeometryData::indexAt(int i)
{
    return &(m_Indices.data()[i]);
}

void GeometryData::appendIndexTriangle(unsigned int i0, unsigned int i1, unsigned int i2)
{
    int currentSize = m_Indices.size();
    m_Indices.resize(currentSize + 3);
    m_Indices[currentSize] = i0;
    m_Indices[currentSize+1] = i1;
    m_Indices[currentSize+2] = i2;

    m_bIndicesStale = true;
}

BoundingBox GeometryData::localBounds() const
{
    QVector3D max, min;

    for (int i = 0; i < vertexCount(); i++)
    {
        const float* pos = vertexAt(i);

        if ( pos[0] < min.x() )
        {
            min.setX(pos[0]);
        }
        else if ( pos[0] > max.x() )
        {
            max.setX(pos[0]);
        }

        if ( pos[1] < min.y() )
        {
            min.setY(pos[1]);
        }
        else if ( pos[1] > max.y() )
        {
            max.setY(pos[1]);
        }

        if ( pos[2] < min.z() )
        {
            min.setZ(pos[2]);
        }
        else if ( pos[2] > max.z() )
        {
            max.setZ(pos[2]);
        }
    }

    return BoundingBox(min,max);
}

const float* GeometryData::vertexAt(int i) const
{
    return m_Vertices.constData() + (i * (formatStride[m_iDataFormat]/sizeof(float)));
}

const unsigned int* GeometryData::indexAt(int i) const
{
    return &(m_Indices.constData()[i]);
}

bool GeometryData::append(const GeometryData &other)
{
    // We can't merge if we have different formats.
    if ( dataFormat() != other.dataFormat() )
        return false;

    // This is where our new vertices start in the list.
    int baseVertex = vertexCount();

    // How many new vertices we need to append.
    int newVertexCount = other.vertexCount();

    // How many floats these new vertices take up.
    int newVertexFloatCount = (formatStride[other.dataFormat()]/sizeof(float)) * newVertexCount;

    // How many bytes these new vertices take up.
    int newVertexByteCount = other.vertexBytes();

    // Resize the vertex list to accommodate the new vertices.
    m_Vertices.resize(m_Vertices.count() + newVertexFloatCount);

    // Copy the new vertices in.
    memcpy(vertexAt(baseVertex), other.vertexAt(0), newVertexByteCount);

    // This is where our new indices start in the list.
    int baseIndex = m_Indices.count();

    // How many new indices we have to append.
    int newIndexCount = other.indexCount();

    // Resize our vector to make space for the new indices.
    m_Indices.resize(m_Indices.count() + newIndexCount);

    // Copy each index in.
    const unsigned int* idxData = other.indexAt(0);
    for ( int i = 0; i < newIndexCount; i++ )
    {
        // Each index must be offset by the baseVertex value.
        m_Indices[baseIndex + i] = idxData[i] + baseVertex;
    }

    m_bVerticesStale = true;
    m_bIndicesStale = true;

    return true;
}

void GeometryData::transform(const QMatrix4x4 &mat)
{
    int floatStride = formatStride[dataFormat()]/sizeof(float);

    for ( int i = 0; i < vertexCount(); i++ )
    {
        int index = i * floatStride;
        QVector4D vec = mat * QVector4D(m_Vertices[index], m_Vertices[index+1], m_Vertices[index+2], 1);
        m_Vertices[index] = vec.x();
        m_Vertices[index+1] = vec.y();
        m_Vertices[index+2] = vec.z();
    }
}
