#ifndef JSONARRAYWRAPPER_H
#define JSONARRAYWRAPPER_H

#include "calliperutil_global.h"
#include <QJsonArray>

namespace CalliperUtil
{
    namespace Json
    {
        // So this class is weird.
        // Keyvalues files can have multiple different entries of the same name,
        // eg. multiple different "solid" items in a VMF.
        // These are converted to arrays when the Keyvalues file is converted
        // to JSON, because JSON objects cannot have multiple entries with the
        // same name. However, if only one key exists, it is left as a
        // JSON object, as this is preferable to converting it to a
        // single-item array.
        // We don't really want to have to check every we want to read an item
        // whether it's a JSON object or whether it's an array.
        // This class takes a QJsonValue in the constructor and converts
        // it to an array representation. This means that by using this class,
        // you don't have to care whether the Keyvalues file had one entry or
        // multiple entries for the item you want to read: you can just treat
        // it as an array.
        class CALLIPERUTILSHARED_EXPORT JsonArrayWrapper : public QJsonArray
        {
        public:
            // For automatic conversion.
            JsonArrayWrapper(const QJsonValue& val);
        };
    }
}

#endif // JSONARRAYWRAPPER_H
