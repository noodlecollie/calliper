#include "vertex3d.h"

namespace NS_RENDERER
{
    Vertex3D::Vertex3D()
        : m_vecPosition(),
          m_vecNormal(),
          m_colColor(QColor::fromRgba(0xffffffff)),
          m_vecTextureCoordinate()
    {

    }

    Vertex3D::Vertex3D(const QVector3D &position, const QVector3D &normal,
                       const QColor &col, const QVector2D &texCoord)
        : m_vecPosition(position),
          m_vecNormal(normal),
          m_colColor(col),
          m_vecTextureCoordinate(texCoord)
    {

    }

    QVector3D Vertex3D::position() const
    {
        return m_vecPosition;
    }

    void Vertex3D::setPosition(const QVector3D &vec)
    {
        m_vecPosition = vec;
    }

    QVector3D Vertex3D::normal() const
    {
        return m_vecNormal;
    }

    void Vertex3D::setNormal(const QVector3D &vec)
    {
        m_vecNormal = vec;
    }

    QColor Vertex3D::color() const
    {
        return m_colColor;
    }

    void Vertex3D::setColor(const QColor &col)
    {
        m_colColor = col;
    }

    QVector2D Vertex3D::textureCoordinate() const
    {
        return m_vecTextureCoordinate;
    }

    void Vertex3D::setTextureCoordinate(const QVector2D &coord)
    {
        m_vecTextureCoordinate = coord;
    }

    Vertex3DValueRef::Vertex3DValueRef(QVector3D &position, QVector3D &normal, QColor &col, QVector2D &texCoord)
        : m_vecPosition(position), m_vecNormal(normal), m_colColor(col), m_vecTextureCoordinate(texCoord)
    {

    }

    QVector3D& Vertex3DValueRef::position()
    {
        return m_vecPosition;
    }

    const QVector3D& Vertex3DValueRef::position() const
    {
        return m_vecPosition;
    }

    QVector3D& Vertex3DValueRef::normal()
    {
        return m_vecNormal;
    }

    const QVector3D& Vertex3DValueRef::normal() const
    {
        return m_vecNormal;
    }

    QColor& Vertex3DValueRef::color()
    {
        return m_colColor;
    }

    const QColor& Vertex3DValueRef::color() const
    {
        return m_colColor;
    }

    QVector2D& Vertex3DValueRef::textureCoordinate()
    {
        return m_vecTextureCoordinate;
    }

    const QVector2D& Vertex3DValueRef::textureCoordinate() const
    {
        return m_vecTextureCoordinate;
    }

    Vertex3DArrayRef::Vertex3DArrayRef()
        : m_pPosition(NULL), m_pNormal(NULL), m_pColor(NULL), m_pTextureCoordinate(NULL)
    {

    }

    Vertex3DArrayRef::Vertex3DArrayRef(float *position, float *normal, float *col, float *texCoord)
        : m_pPosition(position), m_pNormal(normal), m_pColor(col), m_pTextureCoordinate(texCoord)
    {

    }

    bool Vertex3DArrayRef::isNull() const
    {
        return m_pPosition == NULL;
    }

    float* Vertex3DArrayRef::position()
    {
        return m_pPosition;
    }

    const float* Vertex3DArrayRef::position() const
    {
        return m_pPosition;
    }

    float Vertex3DArrayRef::position(int index) const
    {
        return m_pPosition[index];
    }

    void Vertex3DArrayRef::setPosition(int index, float value)
    {
        m_pPosition[index] = value;
    }

    float* Vertex3DArrayRef::normal()
    {
        return m_pNormal;
    }

    const float* Vertex3DArrayRef::normal() const
    {
        return m_pNormal;
    }

    float Vertex3DArrayRef::normal(int index) const
    {
        return m_pNormal[index];
    }

    void Vertex3DArrayRef::setNormal(int index, float value)
    {
        m_pNormal[index] = value;
    }

    float* Vertex3DArrayRef::color()
    {
        return m_pColor;
    }

    const float* Vertex3DArrayRef::color() const
    {
        return m_pColor;
    }

    float Vertex3DArrayRef::color(int index) const
    {
        return m_pColor[index];
    }

    void Vertex3DArrayRef::setColor(int index, float value)
    {
        m_pColor[index] = value;
    }

    float* Vertex3DArrayRef::textureCoordinate()
    {
        return m_pTextureCoordinate;
    }

    const float* Vertex3DArrayRef::textureCoordinate() const
    {
        return m_pTextureCoordinate;
    }

    float Vertex3DArrayRef::textureCoordinate(int index) const
    {
        return m_pTextureCoordinate[index];
    }

    void Vertex3DArrayRef::setTextureCoordinate(int index, float value)
    {
        m_pTextureCoordinate[index] = value;
    }
}
