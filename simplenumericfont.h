#ifndef SIMPLENUMERICFONT_H
#define SIMPLENUMERICFONT_H

#include <QVector2D>
#include <QSharedPointer>

class QOpenGLTexture;
class GeometryData;

class SimpleNumericFont
{
public:
    SimpleNumericFont();
    ~SimpleNumericFont();

    void draw(const QByteArray &numericString, const QSize screenSize, const QSize &charSize, const QPoint &begin);

private:
    static void addQuad(GeometryData* data, int number);
    static QVector2D bottomLeftUV(int number);

    QSharedPointer<QOpenGLTexture> m_pTexture;
    GeometryData*   m_pGeometry;
};

#endif // SIMPLENUMERICFONT_H
