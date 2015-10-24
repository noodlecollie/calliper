#ifndef CVERTEXBUNDLE_H
#define CVERTEXBUNDLE_H

#include <QObject>
#include <QVector>
#include <QVector3D>
#include <QList>
#include "cbaserenderer.h"

class CVertexBundle : public QObject
{
    Q_OBJECT
public:
    explicit CVertexBundle(QObject *parent = 0);

    // Setting the interleaving format does not resize the vector.
    CBaseRenderer::InterleavingFormat interleavingFormat() const;
    void setInterleavingFormat(CBaseRenderer::InterleavingFormat format);

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
    CBaseRenderer::InterleavingFormat      m_iInterleavingFormat;
};

#endif // CVERTEXBUNDLE_H
