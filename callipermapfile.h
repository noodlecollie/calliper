#ifndef CALLIPERMAPFILE_H
#define CALLIPERMAPFILE_H

#include <QString>
#include <QJsonObject>

class MapDocument;

class CalliperMapFile
{
public:
    enum FileFormat
    {
        Binary,
        IndentedJson,
        CompactJson
    };

    CalliperMapFile(const QString &filename, MapDocument* document);

    QString filename() const;
    void setFilename(const QString &fname);

    MapDocument* document() const;
    void setDocument(MapDocument* document);

    bool saveToFile(FileFormat format);
    bool loadFromFile(FileFormat format);

private:
    void insertMetadata();
    void insertSerialisedDocument();

    QString         m_szFilename;
    MapDocument*    m_pDocument;
    QJsonObject     m_RootObject;
};

#endif // CALLIPERMAPFILE_H
