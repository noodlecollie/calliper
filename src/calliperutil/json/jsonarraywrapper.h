#ifndef JSONARRAYWRAPPER_H
#define JSONARRAYWRAPPER_H

#include "calliperutil_global.h"
#include <QJsonArray>

namespace CalliperUtil
{
    namespace Json
    {
        class CALLIPERUTILSHARED_EXPORT JsonArrayWrapper : public QJsonArray
        {
        public:
            // For automatic conversion.
            JsonArrayWrapper(const QJsonValue& val);
        };
    }
}

#endif // JSONARRAYWRAPPER_H
