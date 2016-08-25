#include "rendermodelbatch.h"

RenderModelBatch::RenderModelBatch(QOpenGLBuffer::UsagePattern usagePattern, QObject *parent)
    : QObject(parent),
    m_iErrorFlags(NoBufferFlag),
    m_iUsagePattern(usagePattern),
    m_iVAOID(0)
{
    for ( int i = 0; i < BufferCount; i++ )
    {
        QOpenGLBuffer buffer(i == IndexBuffer ? QOpenGLBuffer::IndexBuffer : QOpenGLBuffer::VertexBuffer);
        buffer.setUsagePattern(m_iUsagePattern);
        m_Buffers.append(buffer);
    }
}

RenderModelBatch::~RenderModelBatch()
{
}

RenderModelBatch::AttributeBufferFlags RenderModelBatch::errorFlags()
{
    AttributeBufferFlags flags = m_iErrorFlags;
    m_iErrorFlags = NoBufferFlag;
    return flags;
}

bool RenderModelBatch::create()
{
    m_iErrorFlags = NoBufferFlag;

    for ( int i = 0; i < BufferCount; i++ )
    {
        if ( !m_Buffers[i].create() )
            setFlag((AttributeBuffer)i);
    }

    return m_iErrorFlags == NoBufferFlag;
}

void RenderModelBatch::destroy()
{
    for ( int i = 0; i < BufferCount; i++ )
    {
        m_Buffers[i].destroy();
    }
}

void RenderModelBatch::setFlag(AttributeBuffer buffer)
{
    switch (buffer)
    {
        case PositionBuffer:
            m_iErrorFlags |= PositionBufferFlag;
            break;

        case NormalBuffer:
            m_iErrorFlags |= NormalBufferFlag;
            break;

        case ColorBuffer:
            m_iErrorFlags |= ColorBufferFlag;
            break;

        case TextureCoordinateBuffer:
            m_iErrorFlags |= TextureCoordinateBufferFlag;
            break;

        case IndexBuffer:
            m_iErrorFlags |= IndexBufferFlag;
            break;

        default:
            break;
    }
}

QOpenGLBuffer::UsagePattern RenderModelBatch::usagePattern() const
{
    return m_iUsagePattern;
}

const QOpenGLBuffer& RenderModelBatch::buffer(AttributeBuffer buffer) const
{
    return m_Buffers.at(buffer);
}

QOpenGLBuffer& RenderModelBatch::buffer(AttributeBuffer buffer)
{
    return m_Buffers[buffer];
}
