#ifndef CALLIPERMAPFILE_H
#define CALLIPERMAPFILE_H

#include "model_global.h"
#include <QString>
#include <QJsonObject>

namespace NS_MODEL
{
    class MapDocument;

    class MODELSHARED_EXPORT CalliperMapFile
    {
    public:
        enum FileFormat
        {
            Binary = 0,
            IndentedJson,
            CompactJson
        };

        static const char* FileFormatStrings[3];

        CalliperMapFile(const QString &filename, MapDocument* document);

        QString filename() const;
        void setFilename(const QString &fname);

        MapDocument* document() const;
        void setDocument(MapDocument* document);

        bool saveToFile(FileFormat format);
        bool loadFromFile();

    private:
        void insertMetadata(FileFormat format);
        void insertSerialisedDocument();

        QString         m_szFilename;
        MapDocument*    m_pDocument;
        QJsonObject     m_RootObject;
    };
}

#endif // CALLIPERMAPFILE_H
