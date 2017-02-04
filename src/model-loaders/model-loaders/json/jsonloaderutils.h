#ifndef JSONLOADERUTILS_H
#define JSONLOADERUTILS_H

#include "model-loaders_global.h"
#include <QJsonDocument>

namespace ModelLoaders
{
    namespace JsonLoaderUtils
    {
        MODELLOADERSSHARED_EXPORT bool loadJsonFile(const QString& filename, QJsonDocument& doc);
    }
}

#endif // JSONLOADERUTILS_H
