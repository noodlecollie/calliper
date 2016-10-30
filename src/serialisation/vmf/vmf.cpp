#include "vmf.h"
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

namespace NS_SERIALISATION
{
    namespace
    {
        class InvalidCoordSyntaxException : public NS_CALLIPERUTIL::CalliperException
        {
        public:
            void raise() const override { throw *this; }
            InvalidCoordSyntaxException* clone() const override { return new InvalidCoordSyntaxException(*this); }

            InvalidCoordSyntaxException(const QString &coord, const QString &err)
                : NS_CALLIPERUTIL::CalliperException(QString("'%1' %2").arg(coord).arg(err))
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

            QStringList list = coord.split(" ");
            if ( list.count() < 3 )
                throw InvalidCoordSyntaxException(coord, "Co-ordinate has too few components.");

            bool success;
            float x, y, z;

            success = false;
            x = list.at(0).toFloat(&success);
            if ( !success )
                throw InvalidCoordSyntaxException(coord, "First co-ordinate component could not be converted to a number.");

            success = false;
            y = list.at(1).toFloat(&success);
            if ( !success )
                throw InvalidCoordSyntaxException(coord, "Second co-ordinate component could not be converted to a number.");

            success = false;
            z = list.at(2).toFloat(&success);
            if ( !success )
                throw InvalidCoordSyntaxException(coord, "Third co-ordinate component could not be converted to a number.");

            return QVector3D(x,y,z);
        }

        void vectorsFromVmfCoords_x(const QString &coords, QVector3D &v0, QVector3D &v1, QVector3D &v2)
        {
            QStringList coordStrings = coords.trimmed().split(" ");
            if ( coordStrings.count() != 3 )
                throw InvalidCoordSyntaxException(coords, "Expected three co-ordinates in group.");

            v0 = vectorFromVmfCoord_x(coordStrings.at(0));
            v1 = vectorFromVmfCoord_x(coordStrings.at(1));
            v2 = vectorFromVmfCoord_x(coordStrings.at(2));
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
                bool success = true;
                for ( int j = 0; j < sides.count(); j++ )
                {
                    QString plane = sides.at(j).toObject().value("plane").toString();
                    QVector3D v0, v1, v2;

                    try
                    {
                        vectorsFromVmfCoords_x(plane, v0, v1, v2);
                    }
                    catch ( NS_CALLIPERUTIL::CalliperException& exception )
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

                    polygons.append(new TexturedWinding(Plane3D(v0, v2, v1), 0));   // TODO: Proper texture
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
