#ifndef GEOMETRYDATA_H
#define GEOMETRYDATA_H

#include <QVector>
#include <QVector2D>
#include <QVector3D>
#include <QColor>
#include <QOpenGLFunctions>
#include <QSharedPointer>
#include "boundingbox.h"

class QOpenGLBuffer;
class ShaderProgram;
class QOpenGLTexture;

#define MAX_GEOM_TEXTURES 4

class GeometryData
{
public:
    GeometryData();
    ~GeometryData();

    // Position must always be the first attribute!
    enum DataFormat
    {
        PositionNormalUV,
        PositionColor
    };

    // Methods for appending vertices with different formats.
    // The data format is automatically set.
    // These should not really be mixed - undefined behaviour may result.
    void appendVertex(const QVector3D &pos, const QVector3D &normal, const QVector2D &uv);
    void appendVertex(const QVector3D &pos, const QColor &col);

    void clearVertices();
    int vertexCount() const;
    int vertexBytes() const;
    void bindVertices(bool bind);
    float* vertexAt(int i);
    const float* vertexAt(int i) const;

    void appendIndex(unsigned int i);
    void appendIndexTriangle(unsigned int i0, unsigned int i1, unsigned int i2);
    void clearIndices();
    int indexCount() const;
    int indexBytes() const;
    void bindIndices(bool bind);
    unsigned int* indexAt(int i);
    const unsigned int* indexAt(int i) const;

    void clear();
    bool isEmpty() const;
    void destroy();

    void upload(bool force = false);
    void draw(int offset = 0, int count = -1);

    QString texture(int index) const;
    void setTexture(int index, const QString &path);

    GLenum drawMode() const;
    void setDrawMode(GLenum mode);
    float lineWidth() const;
    void setLineWidth(float width);

    DataFormat dataFormat() const;
    void setDataFormat(DataFormat format);
    void applyDataFormat(ShaderProgram* program);

    QSharedPointer<QOpenGLTexture> localTexture() const;
    void setLocalTexture(const QSharedPointer<QOpenGLTexture> &tex);
    bool hasLocalTexture() const;

    QString shaderOverride() const;
    void setShaderOverride(const QString &shader);

    int vertexFormatBytes() const;

    // This is generated on demand.
    // Always includes the origin.
    BoundingBox localBounds() const;

private:
    QVector<float>          m_Vertices;
    bool                    m_bVerticesStale;

    QVector<unsigned int>   m_Indices;
    bool                    m_bIndicesStale;

    QOpenGLBuffer*  m_pVertexBuffer;
    QOpenGLBuffer*  m_pIndexBuffer;

    QString         m_Textures[MAX_GEOM_TEXTURES];
    GLenum          m_iDrawMode;
    float           m_flLineWidth;
    DataFormat      m_iDataFormat;

    QSharedPointer<QOpenGLTexture>  m_pLocalTexture;
    QString m_szShaderOverride;
};

#endif // GEOMETRYDATA_H
