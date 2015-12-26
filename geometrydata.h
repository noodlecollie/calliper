#ifndef GEOMETRYDATA_H
#define GEOMETRYDATA_H

#include <QVector>
#include <QVector2D>
#include <QVector3D>
#include <QColor>
#include <QOpenGLFunctions>

class QOpenGLBuffer;

#define MAX_GEOM_TEXTURES 4

class GeometryData
{
public:
    GeometryData();
    ~GeometryData();

    // Methods for appending vertices with different formats.
    // These should not really be mixed - undefined behaviour may result.
    void appendVertex(const QVector3D &pos, const QVector3D &normal, const QVector2D &uv);
    void appendVertex(const QVector3D &pos, const QColor &col);

    void clearVertices();
    int vertexCount() const;
    int vertexBytes() const;
    void bindVertices(bool bind);

    void appendIndex(unsigned int i);
    void clearIndices();
    int indexCount() const;
    int indexBytes() const;
    void bindIndices(bool bind);

    void clear();
    bool isEmpty() const;
    void destroy();

    void upload(bool force = false);
    void draw();

    QString texture(int index) const;
    void setTexture(int index, const QString &path);

    GLenum drawMode() const;
    void setDrawMode(GLenum mode);

private:
    QVector<float>          m_Vertices;
    bool                    m_bVerticesStale;

    QVector<unsigned int>   m_Indices;
    bool                    m_bIndicesStale;

    QOpenGLBuffer*  m_pVertexBuffer;
    QOpenGLBuffer*  m_pIndexBuffer;

    QString         m_Textures[MAX_GEOM_TEXTURES];
    GLenum          m_iDrawMode;
};

#endif // GEOMETRYDATA_H
