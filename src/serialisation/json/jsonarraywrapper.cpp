#include "jsonarraywrapper.h"

namespace NS_SERIALISATION
{
    namespace Json
    {
        JsonArrayWrapper::JsonArrayWrapper(const QJsonValue &val)
            : QJsonArray(val.isArray() ? val.toArray() : QJsonArray({val}))
        {

        }
    }
}
