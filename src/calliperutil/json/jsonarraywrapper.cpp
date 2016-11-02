#include "jsonarraywrapper.h"

namespace CalliperUtil
{
    namespace Json
    {
        JsonArrayWrapper::JsonArrayWrapper(const QJsonValue &val)
            : QJsonArray(val.isArray() ? val.toArray() : QJsonArray({val}))
        {

        }
    }
}
