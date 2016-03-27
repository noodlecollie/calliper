#include "geometryfactory.h"
#include <QVector3D>
#include <QVector2D>
#include "geometrydata.h"
#include "boundingbox.h"
#include "shaders.h"
#include <QtMath>
#include "objfileparser.h"
#include <QFile>
#include "callipermath.h"

namespace GeometryFactory
{
    GeometryData* cube(float radius)
    {
        GeometryData* d = new GeometryData();

        for (int i = 0; i < 6; i++)
        {
            QVector3D normal, u, v;
            switch (i)
            {
            case 0: // X
                normal = QVector3D(1,0,0);
                u = QVector3D(0,1,0);
                v = QVector3D(0,0,1);
                break;
            case 1: // -X
                normal = QVector3D(-1,0,0);
                u = QVector3D(0,-1,0);
                v = QVector3D(0,0,1);
                break;
            case 2: // Y:
                normal = QVector3D(0,1,0);
                u = QVector3D(-1,0,0);
                v = QVector3D(0,0,1);
                break;
            case 3: // -Y:
                normal = QVector3D(0,-1,0);
                u = QVector3D(1,0,0);
                v = QVector3D(0,0,1);
                break;
            case 4: // Z:
                normal = QVector3D(0,0,1);
                u = QVector3D(1,0,0);
                v = QVector3D(0,1,0);
                break;
            case 5: // -Z:
                normal = QVector3D(0,0,-1);
                u = QVector3D(-1,0,0);
                v = QVector3D(0,1,0);
                break;
            default:
                break;
            }

            d->appendVertex((-radius*u) + (-radius*v) + (radius*normal), normal, QVector2D(0,0));
            d->appendVertex((radius*u) + (-radius*v) + (radius*normal), normal, QVector2D(1,0));
            d->appendVertex((radius*u) + (radius*v) + (radius*normal), normal, QVector2D(1,1));
            d->appendVertex((-radius*u) + (radius*v) + (radius*normal), normal, QVector2D(0,1));

            int index = 4*i;
            d->appendIndex(index);
            d->appendIndex(index+1);
            d->appendIndex(index+2);
            d->appendIndex(index);
            d->appendIndex(index+2);
            d->appendIndex(index+3);
        }

        return d;
    }

    GeometryData* cubeFullAtts(float radius)
    {
        GeometryData* d = new GeometryData();
        QColor col = QColor::fromRgb(0xffff0000);

        for (int i = 0; i < 6; i++)
        {
            QVector3D normal, u, v;
            switch (i)
            {
            case 0: // X
                normal = QVector3D(1,0,0);
                u = QVector3D(0,1,0);
                v = QVector3D(0,0,1);
                break;
            case 1: // -X
                normal = QVector3D(-1,0,0);
                u = QVector3D(0,-1,0);
                v = QVector3D(0,0,1);
                break;
            case 2: // Y:
                normal = QVector3D(0,1,0);
                u = QVector3D(-1,0,0);
                v = QVector3D(0,0,1);
                break;
            case 3: // -Y:
                normal = QVector3D(0,-1,0);
                u = QVector3D(1,0,0);
                v = QVector3D(0,0,1);
                break;
            case 4: // Z:
                normal = QVector3D(0,0,1);
                u = QVector3D(1,0,0);
                v = QVector3D(0,1,0);
                break;
            case 5: // -Z:
                normal = QVector3D(0,0,-1);
                u = QVector3D(-1,0,0);
                v = QVector3D(0,1,0);
                break;
            default:
                break;
            }

            d->appendVertex((-radius*u) + (-radius*v) + (radius*normal), normal, QVector2D(0,0), col);
            d->appendVertex((radius*u) + (-radius*v) + (radius*normal), normal, QVector2D(1,0), col);
            d->appendVertex((radius*u) + (radius*v) + (radius*normal), normal, QVector2D(1,1), col);
            d->appendVertex((-radius*u) + (radius*v) + (radius*normal), normal, QVector2D(0,1), col);

            int index = 4*i;
            d->appendIndex(index);
            d->appendIndex(index+1);
            d->appendIndex(index+2);
            d->appendIndex(index);
            d->appendIndex(index+2);
            d->appendIndex(index+3);
        }

        return d;
    }

    GeometryData* cubeSolidColor(float radius, const QColor &col, bool nullNormals)
    {
        GeometryData* d = new GeometryData();
        d->setShaderOverride(PerVertexColorShader::staticName());

        for (int i = 0; i < 6; i++)
        {
            QVector3D normal, u, v;
            switch (i)
            {
            case 0: // X
                normal = QVector3D(1,0,0);
                u = QVector3D(0,1,0);
                v = QVector3D(0,0,1);
                break;
            case 1: // -X
                normal = QVector3D(-1,0,0);
                u = QVector3D(0,-1,0);
                v = QVector3D(0,0,1);
                break;
            case 2: // Y:
                normal = QVector3D(0,1,0);
                u = QVector3D(-1,0,0);
                v = QVector3D(0,0,1);
                break;
            case 3: // -Y:
                normal = QVector3D(0,-1,0);
                u = QVector3D(1,0,0);
                v = QVector3D(0,0,1);
                break;
            case 4: // Z:
                normal = QVector3D(0,0,1);
                u = QVector3D(1,0,0);
                v = QVector3D(0,1,0);
                break;
            case 5: // -Z:
                normal = QVector3D(0,0,-1);
                u = QVector3D(-1,0,0);
                v = QVector3D(0,1,0);
                break;
            default:
                break;
            }

            d->appendVertex((-radius*u) + (-radius*v) + (radius*normal), nullNormals ? QVector3D() : normal, col);
            d->appendVertex((radius*u) + (-radius*v) + (radius*normal), nullNormals ? QVector3D() : normal, col);
            d->appendVertex((radius*u) + (radius*v) + (radius*normal), nullNormals ? QVector3D() : normal, col);
            d->appendVertex((-radius*u) + (radius*v) + (radius*normal), nullNormals ? QVector3D() : normal, col);

            int index = 4*i;
            d->appendIndex(index);
            d->appendIndex(index+1);
            d->appendIndex(index+2);
            d->appendIndex(index);
            d->appendIndex(index+2);
            d->appendIndex(index+3);
        }

        return d;
    }

    GeometryData* triangleQuad(float radius)
    {
        GeometryData* geometry = new GeometryData();
        geometry->appendVertex(QVector3D(-radius,-radius,0), QVector3D(0,0,1), QVector2D(0,0));
        geometry->appendVertex(QVector3D(radius,-radius,0), QVector3D(0,0,1), QVector2D(1,0));
        geometry->appendVertex(QVector3D(radius,radius,0), QVector3D(0,0,1), QVector2D(1,1));
        geometry->appendVertex(QVector3D(-radius,radius,0), QVector3D(0,0,1), QVector2D(0,1));
        geometry->appendIndex(0);
        geometry->appendIndex(1);
        geometry->appendIndex(2);
        geometry->appendIndex(0);
        geometry->appendIndex(2);
        geometry->appendIndex(3);
        return geometry;
    }

    GeometryData* lineRect(float radius, const QColor &col)
    {
        GeometryData* geometry = new GeometryData();
        geometry->setDrawMode(GL_LINE_LOOP);
        geometry->appendVertex(QVector3D(-radius, -radius, 0), QVector3D(), col);
        geometry->appendVertex(QVector3D(radius, -radius, 0), QVector3D(), col);
        geometry->appendVertex(QVector3D(radius, radius, 0), QVector3D(), col);
        geometry->appendVertex(QVector3D(-radius, radius, 0), QVector3D(), col);
        geometry->appendIndex(0);
        geometry->appendIndex(1);
        geometry->appendIndex(2);
        geometry->appendIndex(3);
        return geometry;
    }

    GeometryData* lineCuboid(const QVector3D &min, const QVector3D &max, const QColor &col)
    {
        QList<QVector3D> corners = BoundingBox::corners(min,max);
        GeometryData* geometry = new GeometryData();
        geometry->setDrawMode(GL_LINES);
        for ( int i = 0; i < corners.count(); i++ )
        {
            geometry->appendVertex(corners.at(i), QVector3D(), col);
        }

        geometry->appendIndex(0);
        geometry->appendIndex(1);
        geometry->appendIndex(1);
        geometry->appendIndex(3);
        geometry->appendIndex(3);
        geometry->appendIndex(2);
        geometry->appendIndex(2);
        geometry->appendIndex(0);

        geometry->appendIndex(4);
        geometry->appendIndex(5);
        geometry->appendIndex(5);
        geometry->appendIndex(7);
        geometry->appendIndex(7);
        geometry->appendIndex(6);
        geometry->appendIndex(6);
        geometry->appendIndex(4);

        geometry->appendIndex(0);
        geometry->appendIndex(4);
        geometry->appendIndex(1);
        geometry->appendIndex(5);
        geometry->appendIndex(2);
        geometry->appendIndex(6);
        geometry->appendIndex(3);
        geometry->appendIndex(7);

        return geometry;
    }

    GeometryData* lineCuboid(const BoundingBox &bbox, const QColor &col)
    {
        return lineCuboid(bbox.min(), bbox.max(), col);
    }

    GeometryData* fromObjFile(const QString &filename, float scale)
    {
        GeometryData* geometry = new GeometryData();

        QList<QVector3D> positions;
        QList<QVector3D> normals;
        QList<QVector2D> uvs;
        QList<unsigned int> indices;
        QString texturePath;

        ObjFileParser parser;
        ObjFileParser::ParseResult result = parser.fillAttributes(filename, positions, normals, uvs, indices, texturePath);
        if ( result.error != ObjFileParser::NoError )
        {
            qWarning() << "Error parsing obj file" << filename
                       << "-" << ObjFileParser::errorString(result);
        }

        Q_ASSERT(positions.count() == normals.count() && normals.count() == uvs.count());

        for ( int i = 0; i < positions.count(); i++ )
        {
            geometry->appendVertex(positions.at(i), normals.at(i), uvs.at(i));
        }

        for ( int i = 0; i < indices.count(); i++ )
        {
            geometry->appendIndex(indices.at(i));
        }

        if ( scale != 1.0f )
        {
            geometry->transform(Math::matrixScaleUniform(scale) * Math::openGLToHammer());
        }
        else
        {
            geometry->transform(Math::openGLToHammer());
        }

        return geometry;
    }
}
