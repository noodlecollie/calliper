#ifndef GEOMETRYDATA_H
#define GEOMETRYDATA_H

#include <QVector>
#include <QVector2D>
#include <QVector3D>

class QOpenGLBuffer;

class GeometryData
{
public:
    GeometryData();
    ~GeometryData();

    void appendVertex(const QVector3D &pos, const QVector3D &normal, const QVector2D &uv);
    void clearVertices();
    int vertexCount() const;
    int vertexBytes() const;
    void bindVertices(bool bind);

    void appendIndex(unsigned int i);
    void clearIndices();
    int indexCount() const;
    int indexBytes() const;
    void bindIndices(bool bind);

    void upload(bool force = false);
    void draw();

private:
    QVector<float>          m_Vertices;
    bool                    m_bVerticesStale;

    QVector<unsigned int>   m_Indices;
    bool                    m_bIndicesStale;

    QOpenGLBuffer*  m_pVertexBuffer;
    QOpenGLBuffer*  m_pIndexBuffer;
};

#endif // GEOMETRYDATA_H
