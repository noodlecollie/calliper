#include "callipermapfile.h"
#include "mapdocument.h"
#include "iserialisable.h"
#include <QSaveFile>
#include <QJsonDocument>

CalliperMapFile::CalliperMapFile(const QString &filename, MapDocument *document)
{
    m_szFilename = filename;
    m_pDocument = document;

    insertMetadata();
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
