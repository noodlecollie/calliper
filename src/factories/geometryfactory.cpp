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

static void appendRectFace(const QVector3D &v0, const QVector3D &v1, const QVector3D &v2, const QVector3D &v3,
                    const QVector3D &normal, const QColor &col, GeometryData* geom)
{
    unsigned int i = geom->vertexCount();
    geom->appendVertex(v0, normal, col);
    geom->appendVertex(v1, normal, col);
    geom->appendVertex(v2, normal, col);
    geom->appendVertex(v3, normal, col);

    geom->appendIndexTriangle(i, i+1, i+2);
    geom->appendIndexTriangle(i, i+2, i+3);
}

static void appendPin(int subdivisions, float radius, const QColor &col, GeometryData* geom, const QMatrix4x4 &transform = QMatrix4x4())
{
    Q_ASSERT(subdivisions >= 3);

    GeometryData temp;
    temp.setDataFormat(GeometryData::PositionNormalColor);

    // Create vertices at the end of the pin.
    // These have x = 1, and y and z vary.
    float angleDelta = 1.0f/(float)subdivisions;
    for ( int i = 0; i < subdivisions; i++ )
    {
        float rot = i * angleDelta * 2.0f * M_PI;
        float yDisplacement = (float)radius * qCos(rot);
        float zDisplacement = (float)radius * qSin(rot);

        // TODO: Normal
        temp.appendVertex(QVector3D(1, yDisplacement, zDisplacement), QVector3D(), col);
    }

    // Create the origin vertex.
    temp.appendVertex(QVector3D(0,0,0), QVector3D(), col);

    // Create each of the outer faces.
    int originIndex = temp.vertexCount() - 1;
    for ( int i = 0; i < subdivisions; i++ )
    {
        int index0 = i;
        int index1 = i == (subdivisions - 1) ? 0 : i+1;
        temp.appendIndexTriangle(originIndex, index1, index0);
    }

    // Create the end face.
    for ( int i = 1; i < subdivisions-1; i++ )
    {
        temp.appendIndexTriangle(0, i, i+1);
    }

    if ( !transform.isIdentity() )
    {
        temp.transform(transform);
    }

    geom->append(temp);
}

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

    GeometryData* cuboidSolidColor(const QVector3D &min, const QVector3D &max, const QColor &col, bool nullNormals)
    {
        QList<QVector3D> corners = BoundingBox::corners(min,max);
        GeometryData* geometry = new GeometryData();

        for ( int i = 0; i < 6; i++ )
        {
            Math::AxisIdentifier axis = (Math::AxisIdentifier)(i%3);
            bool negativeAxis = i >= 3;

            QVector3D normal;
            if ( !nullNormals )
            {
                switch (axis)
                {
                    case Math::AxisX:
                        normal = QVector3D(1,0,0);
                        break;

                    case Math::AxisY:
                        normal = QVector3D(0,1,0);
                        break;

                    case Math::AxisZ:
                        normal = QVector3D(0,0,1);
                        break;

                    default:
                        break;
                }

                if ( negativeAxis )
                    normal *= -1;
            }

            // Build the face for this axis.
            const int* indices = BoundingBox::cornerVerticesForFace(axis);
            int offset = negativeAxis ? 4 : 0;

            appendRectFace(corners.at(indices[offset+0]),
                    corners.at(indices[offset+1]),
                    corners.at(indices[offset+2]),
                    corners.at(indices[offset+3]),
                    normal, col, geometry);
        }

        return geometry;
    }

    GeometryData* cuboidSolidColor(const BoundingBox &bbox, const QColor &col, bool nullNormals)
    {
        return cuboidSolidColor(bbox.min(), bbox.max(), col, nullNormals);
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

    GeometryData* singleFlatArrow(float length, float width, float headProportion, const QColor &col, bool nullNormals)
    {
        GeometryData* geom = new GeometryData();
        geom->setDrawMode(GL_TRIANGLES);
        geom->setShaderOverride(PerVertexColorShader::staticName());

        // Shaft
        geom->appendVertex(QVector3D(0.0f, width/4.0f, 0.0f),
                           QVector3D(0,0, nullNormals ? 0 : 1),
                           col);
        geom->appendVertex(QVector3D(0.0f, -width/4.0f, 0.0f),
                           QVector3D(0,0,1),
                           col);
        geom->appendVertex(QVector3D(length*(1-headProportion), -width/4.0f, 0.0f),
                           QVector3D(0,0, nullNormals ? 0 : 1),
                           col);
        geom->appendVertex(QVector3D(length*(1-headProportion), width/4.0f, 0.0f),
                           QVector3D(0,0, nullNormals ? 0 : 1),
                           col);

        // Head
        geom->appendVertex(QVector3D(length*(1-headProportion), width/2.0f, 0.0f),
                           QVector3D(0,0, nullNormals ? 0 : 1),
                           col);
        geom->appendVertex(QVector3D(length*(1-headProportion), -width/2.0f, 0.0f),
                           QVector3D(0,0, nullNormals ? 0 : 1),
                           col);
        geom->appendVertex(QVector3D(length, 0.0f, 0.0f),
                           QVector3D(0,0, nullNormals ? 0 : 1),
                           col);

        return geom;
    }

    GeometryData* flatArrow(float length, float width, float headProportion, const QColor &col, bool nullNormals)
    {
        GeometryData* geom = new GeometryData();
        geom->setDrawMode(GL_TRIANGLES);
        geom->setShaderOverride(PerVertexColorShader::staticName());
        geom->setDataFormat(GeometryData::PositionNormalColor);

        // Get the first side.
        GeometryData* side1 = singleFlatArrow(length, width, headProportion, col, nullNormals);
        geom->append(*side1);

        // If we have null normals, we can just create double-sided triangles.
        if ( nullNormals )
        {
            geom->appendIndexTriangle(0,1,2,true);
            geom->appendIndexTriangle(0,2,3,true);
            geom->appendIndexTriangle(4,5,6,true);
        }

        // Otherwise we need a new side.
        else
        {
            side1->transform(Math::matrixRotateX(qDegreesToRadians(180.0f)));
            geom->append(*side1);

            for ( int i = 0; i < 2; i++ )
            {
                int b = i*7;
                geom->appendIndexTriangle(b+0,b+1,b+2);
                geom->appendIndexTriangle(b+0,b+2,b+3);
                geom->appendIndexTriangle(b+4,b+5,b+6);
            }
        }

        delete side1;
        return geom;
    }

    GeometryData* hexPin(int subdivisions, float pinRadius)
    {
        GeometryData* geom = new GeometryData();
        geom->setShaderOverride(PerVertexColorShader::staticName());
        geom->setDataFormat(GeometryData::PositionNormalColor);

        appendPin(subdivisions, pinRadius, QColor::fromRgb(0xffff0000), geom, QMatrix4x4());
        appendPin(subdivisions, pinRadius, QColor::fromRgb(0xffff0000), geom, Math::matrixRotateZ(qDegreesToRadians(180.0f)));

        appendPin(subdivisions, pinRadius, QColor::fromRgb(0xff00ff00), geom, Math::matrixRotateZ(qDegreesToRadians(90.0f)));
        appendPin(subdivisions, pinRadius, QColor::fromRgb(0xff00ff00), geom, Math::matrixRotateZ(qDegreesToRadians(-90.0f)));

        appendPin(subdivisions, pinRadius, QColor::fromRgb(0xff0000ff), geom, Math::matrixRotateY(qDegreesToRadians(90.0f)));
        appendPin(subdivisions, pinRadius, QColor::fromRgb(0xff0000ff), geom, Math::matrixRotateY(qDegreesToRadians(-90.0f)));

        return geom;
    }
}
