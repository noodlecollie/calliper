#include "geometrydata.h"
#include <QOpenGLBuffer>
#include <QOpenGLFunctions_4_1_Core>
#include "resourcemanager.h"
#include "shaderprogram.h"
#include <QtDebug>
#include <QOpenGLTexture>

static int numComponents[] = {
    3,  // Position
    3,  // Normal
    4,  // Color
    2,  // UV
};

static const int formatStride[] = {
    8*sizeof(float),    // PositionNormalUV
    7*sizeof(float),    // PositionColor
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

        case GeometryData::PositionColor:
        {
            switch (att)
            {
            case ShaderProgram::Position:
                return 0;

            case ShaderProgram::Color:
                return 3*sizeof(float);

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

GeometryData::GeometryData()
{
    m_bVerticesStale = true;
    m_bIndicesStale = true;

    m_pVertexBuffer = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    m_pIndexBuffer = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);

    m_iDrawMode = GL_TRIANGLES;
    m_iDataFormat = PositionNormalUV;
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
    m_iDataFormat = PositionColor;
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

        case PositionColor:
        {
            program->setAttributeFormat(
                        ShaderProgram::Position,
                        numComponents[ShaderProgram::Position],
                        formatStride[m_iDataFormat],
                        formatOffset(m_iDataFormat, ShaderProgram::Position));

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
