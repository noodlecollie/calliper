#ifndef VALVEMAPFILE_H
#define VALVEMAPFILE_H

#include "model_global.h"
#include <QString>

namespace NS_MODEL
{
    class MapDocument;

    class ValveMapFile
    {
    public:
        ValveMapFile(const QString &filename, MapDocument* document);

        QString filename() const;
        void setFilename(const QString &fname);

        MapDocument* document() const;
        void setDocument(MapDocument* document);

        bool saveToFile();
        bool loadFromFile();

    private:
        QString         m_szFilename;
        MapDocument*    m_pDocument;
    };
}

#endif // VALVEMAPFILE_H
