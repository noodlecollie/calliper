#include "geometryfactory.h"
#include <QtDebug>

namespace Model
{
    namespace GeometryFactory
    {
        void cube(RenderSystem::GeometryBuilder &builder, float radius, const QColor &color)
        {
            using namespace RenderSystem;

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

                QSharedPointer<GeometrySection> section = builder.createNewSection();

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
            }
        }

        void wireframeCube(RenderSystem::GeometryBuilder &builder, float radius, const QColor &color)
        {
            using namespace RenderSystem;

            QSharedPointer<GeometrySection> section = builder.createNewSection();
            section->setDrawMode(GeometrySection::DrawLines);

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

            section->addIndexPair(0,1);
            section->addIndexPair(1,3);
            section->addIndexPair(3,2);
            section->addIndexPair(2,0);

            section->addIndexPair(4,5);
            section->addIndexPair(5,7);
            section->addIndexPair(7,6);
            section->addIndexPair(6,4);

            section->addIndexPair(0,4);
            section->addIndexPair(1,5);
            section->addIndexPair(2,6);
            section->addIndexPair(3,7);
        }
    }
}
