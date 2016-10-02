#include "geometryfactory.h"

namespace NS_MODEL
{
    namespace GeometryFactory
    {
        void cube(NS_RENDERER::GeometryBuilder &builder, float radius)
        {
            using namespace NS_RENDERER;

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

                GeometrySection& section = builder.nextEmptySection();

                section.addPosition((-radius*u) + (-radius*v) + (radius*normal));
                section.addPosition((radius*u) + (-radius*v) + (radius*normal));
                section.addPosition((radius*u) + (radius*v) + (radius*normal));
                section.addPosition((-radius*u) + (radius*v) + (radius*normal));

                section.addNormal(normal);
                section.addNormal(normal);
                section.addNormal(normal);
                section.addNormal(normal);

                section.addTextureCoordinate(QVector2D(0,0));
                section.addTextureCoordinate(QVector2D(1,0));
                section.addTextureCoordinate(QVector2D(1,1));
                section.addTextureCoordinate(QVector2D(0,1));

                section.addIndexTriangle(0,1,2);
                section.addIndexTriangle(0,2,3);
            }
        }
    }
}
