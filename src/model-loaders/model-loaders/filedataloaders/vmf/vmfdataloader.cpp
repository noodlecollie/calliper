#include "vmfdataloader.h"
#include "model/filedatamodels/map/mapfiledatamodel.h"
#include "calliperutil/exceptions/calliperexception.h"
#include "model/math/texturedwinding.h"
#include "calliperutil/json/jsonarraywrapper.h"
#include "model/genericbrush/genericbrush.h"
#include "model/factories/genericbrushfactory.h"
#include <QJsonObject>
#include "calliperutil/general/generalutil.h"
#include "model/global/resourceenvironment.h"
#include <QTextStream>
#include "file-formats/keyvalues/keyvaluesparser.h"

namespace
{
    class InvalidCoordSyntaxException : public CalliperUtil::CalliperException
    {
    public:
        void raise() const override
        {
            throw *this;

        }
        InvalidCoordSyntaxException* clone() const override
        {
            return new InvalidCoordSyntaxException(*this);
        }

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

    inline void setErrorString(QString* string, const QString& error)
    {
        if ( string )
        {
            *string = error;
        }
    }
}

namespace ModelLoaders
{
    VmfDataLoader::VmfDataLoader()
        : BaseFileLoader(),
          m_iSuccess(Success)
    {

    }

    BaseFileLoader::LoaderType VmfDataLoader::type() const
    {
        return VmfLoader;
    }

    Model::MapFileDataModel* VmfDataLoader::vmfDataModel() const
    {
        return static_cast<Model::MapFileDataModel*>(m_pDataModel);
    }

    bool VmfDataLoader::setDataModel(Model::BaseFileDataModel *model)
    {
        if ( !model || model->type() != Model::BaseFileDataModel::MapModel )
        {
            return false;
        }

        m_pDataModel = model;
        return true;
    }

    BaseFileLoader::SuccessCode VmfDataLoader::load(const QString &filePath, QString *errorString)
    {
        clearInternalState();

        QFile file(filePath);
        if ( !file.open(QIODevice::ReadOnly) )
        {
            setErrorString(errorString, "Unable to open file for reading.");
            return Failure;
        }

        QByteArray fileData = file.readAll();
        file.close();

        QJsonDocument document = createDocument(fileData, errorString);
        if ( document.isNull() )
        {
            return Failure;
        }

        createBrushes(document);

        if ( m_iSuccess != Success && errorString )
        {
            *errorString = m_Errors.join('\n');
        }

        return m_iSuccess;
    }

    BaseFileLoader::SuccessCode VmfDataLoader::save(const QString &filePath, QString *errorString)
    {
        clearInternalState();

        // TODO: We can't write VMFs yet.
        Q_UNUSED(filePath);
        Q_UNUSED(errorString);
        Q_ASSERT_X(false, Q_FUNC_INFO, "Can't save VMFs yet.");
        return Failure;
    }

    void VmfDataLoader::clearInternalState()
    {
        m_Errors.clear();
        m_iSuccess = Success;
    }

    QJsonDocument VmfDataLoader::createDocument(const QByteArray &vmfData, QString *errorString)
    {
        return FileFormats::KeyValuesParser(vmfData).toJsonDocument(errorString);
    }

    void VmfDataLoader::createBrushes(const QJsonDocument &doc)
    {
        QJsonObject world = doc.object().value("world").toObject();
        CalliperUtil::Json::JsonArrayWrapper solids = world.value("solid");

        for ( int i = 0; i < solids.count(); i++ )
        {
            createBrushForSolid(solids.at(i).toObject());
        }
    }

    void VmfDataLoader::createBrushForSolid(const QJsonObject &solid)
    {
        using namespace Model;
        using namespace CalliperUtil;

        QList<TexturedWinding*> polygons;
        QJsonArray sides = solid.value("side").toArray();
        int solidId = solid.value("id").toInt();

        for ( int j = 0; j < sides.count(); j++ )
        {
            TexturedWinding* winding = createSide(sides.at(j).toObject(), solidId);
            if ( winding )
            {
                polygons.append(winding);
            }
            else
            {
                qDeleteAll(polygons);
                m_iSuccess = PartialSuccess;
                return;
            }
        }

        GenericBrush* planeBrush = GenericBrushFactory::createBrushFromWindingGroup(vmfDataModel()->scene()->rootObject(), polygons);
        planeBrush->setObjectName(QString("planeBrush%0").arg(solidId));

        qDeleteAll(polygons);
    }

    Model::TexturedWinding* VmfDataLoader::createSide(const QJsonObject& side, int brushId)
    {
        using namespace Model;
        using namespace CalliperUtil;

        QString materialPath = CalliperUtil::General::normaliseResourcePathSeparators(side.value("material").toString().toLower());
        QString plane = side.value("plane").toString();

        QVector3D v0, v1, v2;

        try
        {
            vectorsFromVmfCoords_x(plane, v0, v1, v2);
        }
        catch ( CalliperUtil::CalliperException& exception )
        {
            QString error = QString("Error parsing plane co-ordinates for side %1: '%2'").arg(side.value("id").toInt()).arg(exception.errorHint());
            addError(brushId, error);
            return nullptr;
        }
        catch (...)
        {
            QString error = QString("Unexpected exception occurred when parsing plane co-ordinates for side %1.").arg(side.value("id").toInt());
            addError(brushId, error);
            return nullptr;
        }

        MaterialStore* materialStore = ResourceEnvironment::globalInstance()->materialStore();

        TexturedWinding* winding = new TexturedWinding(Plane3D(v0, v2, v1), materialStore->getMaterialId(materialPath));
        Q_ASSERT(!QVector3D::crossProduct(v1 - v0, v2 - v0).isNull());

        return winding;
    }

    void VmfDataLoader::addError(int brushId, const QString &error)
    {
        m_Errors.append(QString("[Brush %1] %2").arg(brushId).arg(error));
    }
}
