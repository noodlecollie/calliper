#include "jsonloaderutils.h"
#include <QFile>

namespace ModelLoaders
{
    namespace JsonLoaderUtils
    {
        bool loadJsonFile(const QString &filename, QJsonDocument &doc)
        {
            QFile file(filename);
            if ( !file.open(QIODevice::ReadOnly) )
                return false;

            QJsonParseError error;
            doc = QJsonDocument::fromJson(file.readAll(), &error);
            if ( error.error != QJsonParseError::NoError )
            {
                doc = QJsonDocument();
                return false;
            }

            file.close();
            return true;
        }
    }
}
