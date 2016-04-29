#ifndef GEOMETRYDATA_H
#define GEOMETRYDATA_H

#include <QVector>
#include <QVector2D>
#include <QVector3D>
#include <QColor>
#include <QOpenGLFunctions>
#include <QSharedPointer>
#include "boundingbox.h"
#include <QMatrix4x4>
#include <QtDebug>
#include "iserialisable.h"

class QOpenGLBuffer;
class ShaderProgram;
class QOpenGLTexture;

#define MAX_GEOM_TEXTURES 4

// TODO: Make this class copy-on-write or something, it's probably quite slow right now.
class GeometryData : public ISerialisable
{
public:
    GeometryData();
    GeometryData(const GeometryData &other);
    GeometryData(const QJsonObject &serialisedData);
    ~GeometryData();

    // Position must always be the first attribute!
    enum DataFormat
    {
        PositionNormalUV,
        PositionNormalColor,
        PositionNormalUVColor,
    };

    // Methods for appending vertices with different formats.
    // The data format is automatically set.
    // It is highly recommended not to mix and match these calls, or undefined behaviour may result.

    // Standard textured polygons.
    void appendVertex(const QVector3D &pos, const QVector3D &normal, const QVector2D &uv);

    // Per-vertex coloured lines
    void appendVertex(const QVector3D &pos, const QVector3D &normal, const QColor &col);

    // Mixture of the two - all data included.
    void appendVertex(const QVector3D &pos, const QVector3D &normal, const QVector2D &uv, const QColor &col);

    void clearVertices();
    int vertexCount() const;
    int vertexBytes() const;
    void bindVertices(bool bind);
    float* vertexAt(int i);
    const float* vertexAt(int i) const;

    void appendIndex(unsigned int i);
    void appendIndexTriangle(unsigned int i0, unsigned int i1, unsigned int i2, bool doubleSided = false);
    void appendIndex(unsigned int i0, unsigned int i1);
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

    bool append(const GeometryData &other);
    void transform(const QMatrix4x4 &mat);

    // Be careful when serialising/deserialising this - very little validation is done.
    // The serialised data should be considered internal and volatile.
    virtual bool serialiseToJson(QJsonObject &obj) const;
    virtual QString serialiseIdentifier() const;

private:
    void initDefaults();

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

QDebug operator<<(QDebug debug, const GeometryData &data);

#endif // GEOMETRYDATA_H
