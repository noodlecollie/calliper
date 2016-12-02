#include "vmfloader.h"
#include <QJsonDocument>
#include "math/texturedwinding.h"
#include "math/modelmath.h"
#include "genericbrush/genericbrush.h"
#include "factories/genericbrushfactory.h"
#include <QJsonObject>
#include <QJsonArray>
#include "json/jsonarraywrapper.h"
#include "exceptions/calliperexception.h"
#include <QtDebug>
#include "scene/scene.h"
#include "stores/texturestore.h"
#include "general/generalutil.h"

namespace ModelLoaders
{
    namespace
    {
        class InvalidCoordSyntaxException : public CalliperUtil::CalliperException
        {
        public:
            void raise() const override { throw *this; }
            InvalidCoordSyntaxException* clone() const override { return new InvalidCoordSyntaxException(*this); }

            InvalidCoordSyntaxException(const QString &coord, const QString &err)
                : CalliperUtil::CalliperException(QString("'%1' %2").arg(coord).arg(err))
            {
            }
        };

        QVector3D vectorFromVmfCoord_x(const QString &coord)
        {
            QString s = coord.trimmed();
            if ( s.startsWith("(") )
            {
                if ( s.length() < 2 )
                    throw InvalidCoordSyntaxException(coord, "Co-ordinate not long enough.");

                s = s.right(s.length()-1);
            }

            if ( s.endsWith(")") )
            {
                if ( s.length() < 2 )
                    throw InvalidCoordSyntaxException(coord, "Co-ordinate not long enough.");

                s = s.left(s.length()-1);
            }

            QStringList list = s.split(" ");
            if ( list.count() < 3 )
                throw InvalidCoordSyntaxException(coord, "Co-ordinate has too few components.");

            bool success;
            float x, y, z;

            success = false;
            x = list.at(0).toFloat(&success);
            if ( !success )
                throw InvalidCoordSyntaxException(coord, QString("First co-ordinate component %1 could not be converted to a number.").arg(list.at(0)));

            success = false;
            y = list.at(1).toFloat(&success);
            if ( !success )
                throw InvalidCoordSyntaxException(coord, QString("Second co-ordinate component %1 could not be converted to a number.").arg(list.at(1)));

            success = false;
            z = list.at(2).toFloat(&success);
            if ( !success )
                throw InvalidCoordSyntaxException(coord, QString("Third co-ordinate component %1 could not be converted to a number.").arg(list.at(2)));

            return QVector3D(x,y,z);
        }

        void vectorsFromVmfCoords_x(const QString &coords, QVector3D &v0, QVector3D &v1, QVector3D &v2)
        {
            QStringList fragments = coords.split(" ");
            if ( fragments.count() != 9 )
                throw InvalidCoordSyntaxException(coords, "Unable to determine co-ordinate triple.");

            v0 = vectorFromVmfCoord_x(fragments.at(0) + " " + fragments.at(1) + " " + fragments.at(2));
            v1 = vectorFromVmfCoord_x(fragments.at(3) + " " + fragments.at(4) + " " + fragments.at(5));
            v2 = vectorFromVmfCoord_x(fragments.at(6) + " " + fragments.at(7) + " " + fragments.at(8));
        }
    }

    namespace VMFLoader
    {
        void createBrushes(const QJsonDocument &doc, Model::SceneObject *parent)
        {
            using namespace Model;
            using namespace CalliperUtil;

            MaterialStore* materialStore = parent->parentScene()->materialStore();

            QJsonObject world = doc.object().value("world").toObject();
            Json::JsonArrayWrapper solids = world.value("solid");

            for ( int i = 0; i < solids.count(); i++ )
            {
                QList<TexturedWinding*> polygons;

                QJsonObject solid = solids.at(i).toObject();
                QJsonArray sides = solid.value("side").toArray();
                bool success = true;
                for ( int j = 0; j < sides.count(); j++ )
                {
                    QJsonObject side = sides.at(j).toObject();
                    QString materialPath = CalliperUtil::General::normaliseResourcePathSeparators(side.value("material").toString().toLower());
                    QString plane = side.value("plane").toString();

                    QVector3D v0, v1, v2;

                    try
                    {
                        vectorsFromVmfCoords_x(plane, v0, v1, v2);
                    }
                    catch ( CalliperUtil::CalliperException& exception )
                    {
                        qWarning() << "Error parsing plane co-ordinates:" << exception.errorHint();
                        success = false;
                        break;
                    }
                    catch (...)
                    {
                        qCritical() << "Unexpected exception occurred when parsing plane co-ordinates!";
                        success = false;
                        break;
                    }

                    polygons.append(new TexturedWinding(Plane3D(v0, v2, v1), materialStore->getMaterialId(materialPath)));
                    Q_ASSERT(!QVector3D::crossProduct(v1 - v0, v2 - v0).isNull());
                }

                if ( !success )
                    continue;

                GenericBrush* planeBrush = GenericBrushFactory::createBrushFromWindingGroup(parent, polygons);
                planeBrush->setObjectName(QString("planeBrush%0").arg(i));

                qDeleteAll(polygons);
            }
        }
    }
}
