#include "callipermapfile.h"
#include "mapdocument.h"
#include "iserialisable.h"
#include <QSaveFile>
#include <QJsonDocument>
#include <QFile>

CalliperMapFile::CalliperMapFile(const QString &filename, MapDocument *document)
{
    m_szFilename = filename;
    m_pDocument = document;
}

QString CalliperMapFile::filename() const
{
    return m_szFilename;
}

void CalliperMapFile::setFilename(const QString &fname)
{
    m_szFilename = fname;
}

MapDocument* CalliperMapFile::document() const
{
    return m_pDocument;
}

void CalliperMapFile::setDocument(MapDocument *document)
{
    m_pDocument = document;
}

void CalliperMapFile::insertMetadata()
{
    m_RootObject.remove("metadata");

    QJsonObject metadata;
    metadata.insert("version", QJsonValue((int)ISerialisable::SERIALISATION_VERSION()));

    m_RootObject.insert("metadata", QJsonValue(metadata));
}

void CalliperMapFile::insertSerialisedDocument()
{
    Q_ASSERT(m_pDocument);

    m_RootObject.remove("document");

    QJsonObject doc;
    m_pDocument->serialiseToJson(doc);

    m_RootObject.insert("document", QJsonValue(doc));
}

bool CalliperMapFile::saveToFile(FileFormat format)
{
    QSaveFile file(m_szFilename);
    if ( !file.open(QIODevice::WriteOnly) )
        return false;

    insertMetadata();
    insertSerialisedDocument();

    QJsonDocument jsonDoc(m_RootObject);

    QByteArray data;
    switch (format)
    {
        case Binary:
            data = jsonDoc.toBinaryData();
            break;

        case IndentedJson:
            data = jsonDoc.toJson(QJsonDocument::Indented);
            break;

        case CompactJson:
            data = jsonDoc.toJson(QJsonDocument::Compact);
            break;

        default:
            file.cancelWriting();
            Q_ASSERT(false);
            return false;
    }

    if ( file.write(data) < 0 )
    {
        file.cancelWriting();
        return false;
    }

    file.commit();
    return true;
}

bool CalliperMapFile::loadFromFile(FileFormat format)
{
    QFile file(m_szFilename);
    if ( !file.open(QIODevice::ReadOnly) )
        return false;

    QJsonDocument jsonDoc;
    switch (format)
    {
    case Binary:
        jsonDoc = QJsonDocument::fromBinaryData(file.readAll());
        break;

    case IndentedJson:
    case CompactJson:
        jsonDoc = QJsonDocument::fromJson(file.readAll());
        break;
    }

    file.close();

    if ( !jsonDoc.isObject() )
        return false;

    // Make sure the version is correct.
    m_RootObject = jsonDoc.object();
    QJsonObject metadata = m_RootObject.value("metadata").toObject();
    if ( metadata.isEmpty() )
        return false;

    QJsonValue vVersion = metadata.value("version");
    if ( !vVersion.isDouble() )
        return false;

    unsigned short version = (unsigned short)vVersion.toInt();
    if ( version != ISerialisable::SERIALISATION_VERSION() )
        return false;

    // Unserialise the document.
    return m_pDocument->unserialiseFromJson(m_RootObject.value("document").toObject());
}
