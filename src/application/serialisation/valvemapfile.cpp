#include "valvemapfile.h"
#include <QFile>
#include "keyvaluesparser.h"
#include <QJsonDocument>
#include "texturedpolygon.h"
#include "mapdocument.h"
#include "mapscene.h"
#include "scenefactory.h"

ValveMapFile::ValveMapFile(const QString &filename, MapDocument *document)
{
    m_szFilename = filename;
    m_pDocument = document;
}

QString ValveMapFile::filename() const
{
    return m_szFilename;
}

void ValveMapFile::setFilename(const QString &fname)
{
    m_szFilename = fname;
}

MapDocument* ValveMapFile::document() const
{
    return m_pDocument;
}

void ValveMapFile::setDocument(MapDocument *document)
{
    m_pDocument = document;
}


bool ValveMapFile::saveToFile()
{
    // TODO
    return false;
}

bool ValveMapFile::loadFromFile()
{
    QFile file(m_szFilename);
    if ( !file.open(QIODevice::ReadOnly) )
        return false;

    QByteArray data = file.readAll();
    file.close();

    KeyValuesParser parser(data);
    QJsonDocument doc = parser.toJsonDocument();

    SceneFactory::populateFromVMF(m_pDocument->scene(), doc);

    return true;
}
