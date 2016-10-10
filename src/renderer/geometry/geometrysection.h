#ifndef GEOMETRYSECTION_H
#define GEOMETRYSECTION_H

#include "renderer_global.h"
#include <QVector2D>
#include <QVector3D>
#include <QVector4D>
#include <QColor>
#include "shaders/shaderdefs.h"
#include <QList>
#include <QVector>
#include "geometry/vertex3d.h"
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include "shaders/vertexformat.h"

namespace NS_RENDERER
{
    class RENDERERSHARED_EXPORT GeometrySection
    {
        friend class GeometryBuilder;
    public:
        enum AttributeType
        {
            PositionAttribute = 0,
            NormalAttribute,
            ColorAttribute,
            TextureCoordinateAttribute,

            AttributeTypeCount
        };

        GeometrySection(quint16 shaderId, quint32 textureId, const VertexFormat &vertexFormat, const QMatrix4x4 modelToWorldMatrix);

        // It's assumed that the number of components for each attribute of
        // successive vertices will be the same (eg. 3 floats each time for position).
        // Don't start adding vertices with different numbers of components,
        // that's just asking for trouble.

        void add(AttributeType att, const float* data, int count);

        void addPosition(const QVector3D &pos);
        void addPosition(const QVector4D &pos);
        void addPositions(const float* data, int count, int components);
        void addNormal(const QVector3D &vec);
        void addColor(const QColor &col);
        void addTextureCoordinate(const QVector2D &coord);
        void addIndex(quint32 idx);
        void addIndexLine(quint32 i0, quint32 i1);
        void addIndexTriangle(quint32 i0, quint32 i1, quint32 i2);
        void addVertex(const Vertex3D &vertex);
        void addVertex(const Vertex3DValueRef &vertex);

        int floatCount(AttributeType att) const;
        int indexCount() const;
        int positionCount() const;    // How many times addPosition() has been called
        bool isEmpty() const;

        const float* vertexConstData(AttributeType type) const;
        const quint32* indexConstData() const;

        const QVector<float>& vertexConstVector(AttributeType type) const;
        const QVector<quint32>& indexConstVector() const;

        bool hasNormals() const;
        bool hasColors() const;
        bool hasTextureCoordinates() const;

        int consolidate(QVector<float> &positions, QVector<float> &normals, QVector<float> &colors,
                        QVector<float> &textureCoordinates, QVector<quint32> &indices) const;

        quint16 shaderId() const;
        void setShaderId(quint16 id);

        quint32 textureId() const;
        void setTextureId(quint32 id);

        GLenum drawMode() const;
        void setDrawMode(GLenum mode);

        float drawWidth() const;
        void setDrawWidth(float width);

        const QMatrix4x4& modelToWorldMatrix() const;
        void setModelToWorldMatrix(const QMatrix4x4 &mat);

        const VertexFormat& vertexFormat() const;
        void setVertexFormat(const VertexFormat &format);

    private:
        void init();

        QList<QVector<float> >  m_Attributes;
        QVector<quint32>        m_Indices;
        int                     m_iPositionCount;

        GLenum  m_iDrawMode;
        float   m_flDrawWidth;
        quint16 m_iShaderId;
        quint32 m_iTextureId;
        VertexFormat m_VertexFormat;
        QMatrix4x4 m_matModelToWorld;
    };

    typedef QList<GeometrySection> GeometrySectionList;
}

#endif // GEOMETRYSECTION_H
