#ifndef RENDERMODELBATCH_H
#define RENDERMODELBATCH_H

#include <QObject>
#include <QOpenGLBuffer>

class RenderModelBatch : public QObject
{
    Q_OBJECT
public:
    RenderModelBatch(QOpenGLBuffer::UsagePattern usagePattern, QObject* parent = 0);
    ~RenderModelBatch();

    enum AttributeBufferFlag
    {
        NoBufferFlag = 0x0,

        PositionBufferFlag = 0x1,
        NormalBufferFlag = 0x2,
        ColorBufferFlag = 0x4,
        TextureCoordinateBufferFlag = 0x8,
        IndexBufferFlag = 0x16,
    };
    Q_DECLARE_FLAGS(AttributeBufferFlags, AttributeBufferFlag)

    enum AttributeBuffer
    {
        PositionBuffer = 0,
        NormalBuffer,
        ColorBuffer,
        TextureCoordinateBuffer,
        IndexBuffer,

        BufferCount,
    };
    Q_ENUMS(AttributeBuffer)

    AttributeBufferFlags errorFlags();
    QOpenGLBuffer::UsagePattern usagePattern() const;

    bool create();
    void destroy();

    const QOpenGLBuffer& buffer(AttributeBuffer buffer) const;
    QOpenGLBuffer& buffer(AttributeBuffer buffer);

private:
    void setFlag(AttributeBuffer buffer);

    AttributeBufferFlags        m_iErrorFlags;
    QList<QOpenGLBuffer>        m_Buffers;
    QOpenGLBuffer::UsagePattern m_iUsagePattern;
    GLuint                      m_iVAOID;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(RenderModelBatch::AttributeBufferFlags)

#endif // RENDERMODELBATCH_H
