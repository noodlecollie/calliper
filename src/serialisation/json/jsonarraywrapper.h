#ifndef JSONARRAYWRAPPER_H
#define JSONARRAYWRAPPER_H

#include "serialisation_global.h"
#include <QJsonArray>

namespace NS_SERIALISATION
{
    namespace Json
    {
        class SERIALISATIONSHARED_EXPORT JsonArrayWrapper : public QJsonArray
        {
        public:
            // For automatic conversion.
            JsonArrayWrapper(const QJsonValue& val);
        };
    }
}

#endif // JSONARRAYWRAPPER_H
