#include "simplenumericfont.h"
#include <QOpenGLTexture>
#include <geometrydata.h>
#include <QtMath>
#include "openglrenderer.h"

#define NUMBER_DOT 10
#define NUMBER_DASH 11

SimpleNumericFont::SimpleNumericFont()
{
    m_pTexture = QSharedPointer<QOpenGLTexture>(new QOpenGLTexture(QImage(":/fonts/numbers_small.png").mirrored()));
    m_pTexture->setMinificationFilter(QOpenGLTexture::Nearest);
    m_pTexture->setMagnificationFilter(QOpenGLTexture::Nearest);
    m_pTexture->setWrapMode(QOpenGLTexture::DirectionS, QOpenGLTexture::ClampToEdge);
    m_pTexture->setWrapMode(QOpenGLTexture::DirectionT, QOpenGLTexture::ClampToEdge);

    m_pGeometry = new GeometryData();
    m_pGeometry->setLocalTexture(m_pTexture);
    for (int i = 0; i <= NUMBER_DASH; i++)
    {
        addQuad(m_pGeometry, i);
    }
}

SimpleNumericFont::~SimpleNumericFont()
{
    delete m_pGeometry;
    // Texture is deleted when it goes out of scope.
}

void SimpleNumericFont::addQuad(GeometryData *data, int number)
{
    QVector2D blUV = bottomLeftUV(number);
    static const QVector2D xDim(1.0f/8.0f, 0);
    static const QVector2D yDim(0, 0.5f);
    int baseIndex = data->vertexCount();

    data->appendVertex(QVector3D(-1,-1,0), QVector3D(0,0,1), blUV);
    data->appendVertex(QVector3D(1,-1,0), QVector3D(0,0,1), blUV + xDim);
    data->appendVertex(QVector3D(1,1,0), QVector3D(0,0,1), blUV + xDim + yDim);
    data->appendVertex(QVector3D(-1,1,0), QVector3D(0,0,1), blUV + yDim);
    data->appendIndex(baseIndex);
    data->appendIndex(baseIndex+1);
    data->appendIndex(baseIndex+2);
    data->appendIndex(baseIndex);
    data->appendIndex(baseIndex+2);
    data->appendIndex(baseIndex+3);
}

QVector2D SimpleNumericFont::bottomLeftUV(int number)
{
    // .
    if ( number == NUMBER_DOT )
    {
        return QVector2D(5.0f/8.0f, 0.5f);
    }
    // -
    else if ( number == NUMBER_DASH )
    {
        return QVector2D(5.0f/8.0f, 0.0f);
    }
    // Anything else
    else
    {
        return QVector2D((number%5)*(1.0f/8.0f), (float)(1-qFloor((float)number/5.0f))/2.0f);
    }
}

void SimpleNumericFont::draw(const QByteArray &numericString, const QSize screenSize, const QSize &charSize, const QPoint &begin)
{
    int offset = 0;

    for (int i = 0; i < numericString.count(); i++)
    {
        char c = numericString.at(i);

        if ( c != '.' && c != '-' && (c < '0' || c > '9') )
            continue;

        int number = c - '0';
        if ( c == '.' )
            number = NUMBER_DOT;
        else if ( c == '-' )
            number = NUMBER_DASH;

        renderer()->drawQuad(m_pGeometry,
                             screenSize,
                             QRect(begin.x() + offset, begin.y(), charSize.width(), charSize.height()),
                             Qt::AlignLeft | Qt::AlignTop,
                             number*6*sizeof(unsigned int),
                             6*sizeof(unsigned int));
        offset += charSize.width();
    }
}
