#ifndef VERTEX3D_H
#define VERTEX3D_H

#include "renderer_global.h"
#include <QVector2D>
#include <QVector3D>
#include <QColor>

namespace Renderer
{
    class RENDERERSHARED_EXPORT Vertex3D
    {
    public:
        Vertex3D();
        Vertex3D(const QVector3D &position, const QVector3D &normal,
                 const QColor &col, const QVector2D &texCoord);

        QVector3D position() const;
        void setPosition(const QVector3D &vec);

        QVector3D normal() const;
        void setNormal(const QVector3D &vec);

        QColor color() const;
        void setColor(const QColor &col);

        QVector2D textureCoordinate() const;
        void setTextureCoordinate(const QVector2D &coord);

    private:
        QVector3D   m_vecPosition;
        QVector3D   m_vecNormal;
        QColor      m_colColor;
        QVector2D   m_vecTextureCoordinate;
    };

    class RENDERERSHARED_EXPORT Vertex3DValueRef
    {
    public:
        Vertex3DValueRef(QVector3D &position, QVector3D &normal, QColor &col, QVector2D &texCoord);

        QVector3D& position();
        const QVector3D& position() const;

        QVector3D& normal();
        const QVector3D& normal() const;

        QColor& color();
        const QColor& color() const;

        QVector2D& textureCoordinate();
        const QVector2D& textureCoordinate() const;

    private:
        QVector3D&  m_vecPosition;
        QVector3D&  m_vecNormal;
        QColor&     m_colColor;
        QVector2D&  m_vecTextureCoordinate;
    };

    // This class can handle any number of floats per attribute.
    // It is up to the caller to ensure that the indices provided
    // when indexing the attributes are valid.
    class RENDERERSHARED_EXPORT Vertex3DArrayRef
    {
    public:
        Vertex3DArrayRef();
        Vertex3DArrayRef(float* position, float* normal, float* col, float* texCoord);

        bool isNull() const;

        float* position();
        const float* position() const;
        float position(int index) const;
        void setPosition(int index, float value);

        float* normal();
        const float* normal() const;
        float normal(int index) const;
        void setNormal(int index, float value);

        float* color();
        const float* color() const;
        float color(int index) const;
        void setColor(int index, float value);

        float* textureCoordinate();
        const float* textureCoordinate() const;
        float textureCoordinate(int index) const;
        void setTextureCoordinate(int index, float value);

    private:
        float* m_pPosition;
        float* m_pNormal;
        float* m_pColor;
        float* m_pTextureCoordinate;
    };
}

#endif // VERTEX3D_H
