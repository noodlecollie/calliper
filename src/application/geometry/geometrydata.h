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
#define MAX_GEOM_SECTIONS 16

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

    // Assumed sections have been set up correctly.
    // Sections are basically offsets and lengths stored
    // within the object. The GeometryData makes no attempt
    // to manage or validate them.
    inline void drawSection(int section)
    {
        draw(m_SectionOffsets[section] * sizeof(unsigned int), m_SectionLengths[section]);
    }

    inline void drawSection(const int* sections, int count)
    {
        for ( int i = 0; i < count; i++ )
        {
            drawSection(sections[i]);
        }
    }

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
    BoundingBox localBounds() const;

    bool append(const GeometryData &other);
    void transform(const QMatrix4x4 &mat);

    // Be careful when serialising/deserialising this - very little validation is done.
    // The serialised data should be considered internal and volatile.
    virtual bool serialiseToJson(QJsonObject &obj) const;
    virtual QString serialiseIdentifier() const;

    unsigned int sectionOffset(int section) const;
    unsigned int sectionLength(int section) const;
    void setSectionOffset(int section, unsigned int offset);
    void setSectionLength(int section, unsigned int length);
    inline void setSection(int section, unsigned int offset, unsigned int length)
    {
        setSectionOffset(section, offset);
        setSectionLength(section, length);
    }
    int sectionCount() const;
    void setSectionCount(int count);

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

    unsigned int    m_SectionOffsets[MAX_GEOM_SECTIONS];    // Offset of beginning of section, in indices.
    unsigned int    m_SectionLengths[MAX_GEOM_SECTIONS];    // Length of section, in indices
    int             m_iSectionCount;
};

QDebug operator<<(QDebug debug, const GeometryData &data);

#endif // GEOMETRYDATA_H
