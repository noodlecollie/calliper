#include "vmf.h"
#include <QJsonDocument>
#include "math/texturedwinding.h"
#include "math/modelmath.h"
#include "genericbrush/genericbrush.h"
#include "factories/genericbrushfactory.h"
#include <QJsonObject>
#include <QJsonArray>
#include "json/jsonarraywrapper.h"

namespace NS_SERIALISATION
{
    namespace
    {
        // TODO: Error handling.
        QVector3D vectorFromVmfCoord(const QString &coord)
        {
            QStringList list = coord.split(" ");
            list[0] = list[0].right(list[0].length() - 1);
            list[2] = list[2].left(list[2].length() - 1);

            return QVector3D(list.at(0).toFloat(),
                             list.at(1).toFloat(),
                             list.at(2).toFloat());
        }

        // TODO: Error handling.
        void vectorsFromVmfCoords(const QString &coords, QVector3D &v0, QVector3D &v1, QVector3D &v2)
        {
            int sep1 = coords.indexOf(')') + 1;
            int sep2 = coords.indexOf(')', sep1) + 1;

            QString coord0 = coords.mid(0, sep1);
            QString coord1 = coords.mid(sep1+1, sep2-sep1-1);
            QString coord2 = coords.mid(sep2+1);

            v0 = vectorFromVmfCoord(coord0);
            v1 = vectorFromVmfCoord(coord1);
            v2 = vectorFromVmfCoord(coord2);
        }
    }

    namespace VMF
    {
        void createBrushes(const QJsonDocument &doc, NS_MODEL::SceneObject *parent)
        {
            using namespace NS_MODEL;

            QJsonObject world = doc.object().value("world").toObject();
            Json::JsonArrayWrapper solids = world.value("solid");

            for ( int i = 0; i < solids.count(); i++ )
            {
                QList<TexturedWinding*> polygons;

                QJsonObject solid = solids.at(i).toObject();
                QJsonArray sides = solid.value("side").toArray();
                for ( int j = 0; j < sides.count(); j++ )
                {
                    QString plane = sides.at(j).toObject().value("plane").toString();
                    QVector3D v0, v1, v2;
                    vectorsFromVmfCoords(plane, v0, v1, v2);
                    polygons.append(new TexturedWinding(Plane3D(v0, v2, v1), 0));   // TODO: Proper texture
                    Q_ASSERT(!QVector3D::crossProduct(v1 - v0, v2 - v0).isNull());
                }

                GenericBrush* planeBrush = GenericBrushFactory::createBrushFromWindingGroup(parent, polygons);
                planeBrush->setObjectName(QString("planeBrush%0").arg(i));

                qDeleteAll(polygons);
            }
        }
    }
}
