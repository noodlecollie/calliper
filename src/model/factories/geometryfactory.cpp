#include "geometryfactory.h"
#include <QtDebug>

namespace NS_MODEL
{
    namespace GeometryFactory
    {
        void cube(Renderer::GeometryBuilder &builder, float radius, const QColor &color)
        {
            using namespace Renderer;

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

                GeometrySection* section = builder.createNewSection();

                section->addPosition((-radius*u) + (-radius*v) + (radius*normal));
                section->addPosition((radius*u) + (-radius*v) + (radius*normal));
                section->addPosition((radius*u) + (radius*v) + (radius*normal));
                section->addPosition((-radius*u) + (radius*v) + (radius*normal));

                section->addNormal(normal);
                section->addNormal(normal);
                section->addNormal(normal);
                section->addNormal(normal);

                section->addColor(color);
                section->addColor(color);
                section->addColor(color);
                section->addColor(color);

                section->addTextureCoordinate(QVector2D(0,0));
                section->addTextureCoordinate(QVector2D(1,0));
                section->addTextureCoordinate(QVector2D(1,1));
                section->addTextureCoordinate(QVector2D(0,1));

                section->addIndexTriangle(0,1,2);
                section->addIndexTriangle(0,2,3);
            }
        }

        void wireframeCube(Renderer::GeometryBuilder &builder, float radius, const QColor &color)
        {
            using namespace Renderer;

            GeometrySection* section = builder.createNewSection();
            section->setDrawMode(GL_LINES);

            section->addPosition(QVector3D(-radius, -radius, -radius));
            section->addPosition(QVector3D(radius,  -radius, -radius));
            section->addPosition(QVector3D(-radius, radius,  -radius));
            section->addPosition(QVector3D(radius,  radius,  -radius));
            section->addPosition(QVector3D(-radius, -radius, radius));
            section->addPosition(QVector3D(radius,  -radius, radius));
            section->addPosition(QVector3D(-radius, radius,  radius));
            section->addPosition(QVector3D(radius,  radius,  radius));

            section->addColor(color);
            section->addColor(color);
            section->addColor(color);
            section->addColor(color);
            section->addColor(color);
            section->addColor(color);
            section->addColor(color);
            section->addColor(color);

            section->addIndexLine(0,1);
            section->addIndexLine(1,3);
            section->addIndexLine(3,2);
            section->addIndexLine(2,0);

            section->addIndexLine(4,5);
            section->addIndexLine(5,7);
            section->addIndexLine(7,6);
            section->addIndexLine(6,4);

            section->addIndexLine(0,4);
            section->addIndexLine(1,5);
            section->addIndexLine(2,6);
            section->addIndexLine(3,7);
        }
    }
}
