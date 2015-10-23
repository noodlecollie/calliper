#ifndef CVERTEXBUNDLE_H
#define CVERTEXBUNDLE_H

#include <QObject>
#include <QVector>
#include <QVector3D>
#include <QList>

class CVertexBundle : public QObject
{
    Q_OBJECT
public:
    explicit CVertexBundle(QObject *parent = 0);

    // Formats for vertex buffers.
    // Attributes are interleaved in the order specified.
    enum InterleavingFormat
    {
        FormatPosition = 0,
        FormatPositionUV
    };

    // Attribute types.
    // The value within this enum corresponds to the expected attribute
    // number used in the shader.
    enum Attribute
    {
        Position     = 0,
        UV          // 1
    };

    // These are all IN BYTES.
    // Offset from the beginning of the buffer for the first instance of the given attribute.
    static int attributeOffset(InterleavingFormat format, Attribute att);

    // How large a given attribute is.
    static int attributeSize(Attribute att);

    // How large an entire collection of attributes is.
    // This is the same as the stride required for the format.
    static int interleavingFormatSize(InterleavingFormat format);

    // Setting the interleaving format does not resize the vector.
    InterleavingFormat interleavingFormat() const;
    void setInterleavingFormat(InterleavingFormat format);

    void clear();
    void clearVertices();
    void clearIndices();

    // Number of vertices (ie. groups of attributes).
    int vertexCount() const;
    // Size of the number of vertices stored, in bytes.
    unsigned int vertexDataSize() const;

    // Number of indices. If 0, indices are not used.
    int indexCount() const;
    // Size of the number of indices stored, in bytes.
    unsigned int indexDataSize() const;

    void appendVertex(const QVector3D &position);
    void appendVertex(const QVector3D &position, const QVector2D &uv);

    void appendIndex(unsigned int idx);

    void setVertexPosition(int vertex, const QVector3D &position);
    void setVertexUV(int vertex, const QVector2D &uv);

    void setIndex(int position, unsigned int value);

    float* vertexData();
    const float* vertexConstData() const;

    unsigned int* indexData();
    const unsigned int* indexConstData() const;

signals:

public slots:

private:
    int floatsPerVertex() const;
    int incrementSize();

    QVector<float>          m_VertexData;
    QVector<unsigned int>   m_IndexData;
    InterleavingFormat      m_iInterleavingFormat;
};

#endif // CVERTEXBUNDLE_H
