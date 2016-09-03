#include "callipermapfile.h"
#include "mapdocument.h"
#include "iserialisable.h"
#include <QSaveFile>
#include <QJsonDocument>
#include <QFile>
#include <QRegularExpression>

namespace NS_MODEL
{
    static const QByteArray HEADER_FORMAT_BINARY("#CMF FORMAT BINARY\n");
    static const QByteArray HEADER_FORMAT_JSON("#CMF FORMAT JSON\n");

    const char* CalliperMapFile::FileFormatStrings[3] =
    {
        "Binary",
        "IndentedJson",
        "CompactJson",
    };

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

    void CalliperMapFile::insertMetadata(FileFormat format)
    {
        m_RootObject.remove("metadata");

        QJsonObject metadata;
        metadata.insert("version", QJsonValue((int)ISerialisable::SERIALISATION_VERSION()));
        metadata.insert("format", QJsonValue(FileFormatStrings[format]));

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

        insertMetadata(format);
        insertSerialisedDocument();

        QJsonDocument jsonDoc(m_RootObject);

        const QByteArray* header;
        QByteArray data;
        switch (format)
        {
        case Binary:
            header = &HEADER_FORMAT_BINARY;
            data = jsonDoc.toBinaryData();
            break;

        case IndentedJson:
            header = &HEADER_FORMAT_JSON;
            data = jsonDoc.toJson(QJsonDocument::Indented);
            break;

        case CompactJson:
            header = &HEADER_FORMAT_JSON;
            data = jsonDoc.toJson(QJsonDocument::Compact);
            break;

        default:
            file.cancelWriting();
            Q_ASSERT(false);
            return false;
        }

        if ( file.write(*header) < 0 || file.write(data) < 0 )
        {
            file.cancelWriting();
            return false;
        }

        file.commit();
        return true;
    }

    bool CalliperMapFile::loadFromFile()
    {
        QFile file(m_szFilename);
        if ( !file.open(QIODevice::ReadOnly) )
            return false;

        QByteArray data = file.readAll();
        file.close();

        // Get the first newline.
        int nl = data.indexOf('\n');
        if ( nl <= 0 )
            return false;

        // Get the format type
        QString header(data.mid(0, nl));
        header = header.trimmed();

        QStringList headerTokens = header.split(QRegularExpression("\\s"));
        if ( headerTokens.count() < 3 )
            return false;

        // Ensure it's a directive.
        if ( headerTokens.at(0) != "#CMF" )
            return false;

        // Ensure it describes the format.
        if ( headerTokens.at(1) != "FORMAT" )
            return false;

        // Get the format.
        bool binary = false;
        if ( headerTokens.at(2) == "BINARY" )
            binary = true;
        else if ( headerTokens.at(2) == "JSON" )
            binary = false;
        else
            return false;

        QJsonDocument jsonDoc = binary ? QJsonDocument::fromBinaryData(data.mid(nl+1)) : QJsonDocument::fromJson(data.mid(nl+1));

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

        FileFormat docFormat = IndentedJson;
        QJsonValue vFormat = metadata.value("format");
        if ( vFormat.isString() )
        {
            QString sFormat = vFormat.toString();
            if ( sFormat == FileFormatStrings[CompactJson] )
                docFormat = CompactJson;
            else if ( sFormat == FileFormatStrings[Binary] )
                docFormat = Binary;
            else
                docFormat = IndentedJson;
        }
        m_pDocument->setFileFormat(docFormat);

        // Unserialise the document.
        return m_pDocument->unserialiseFromJson(m_RootObject.value("document").toObject());
    }
}
