#include "geometryfactory.h"
#include <QVector3D>
#include <QVector2D>

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
}
