#include "jsonutil.h"

namespace JsonUtil
{
    void colorToJsonArray(const QColor &col, QJsonArray &arr)
    {
        arr.append(col.red());
        arr.append(col.green());
        arr.append(col.blue());
        arr.append(col.alpha());
    }

    QColor jsonArrayToColor(const QJsonArray &arr)
    {
        return QColor(arr.at(0).toInt(),
                      arr.at(1).toInt(),
                      arr.at(2).toInt(),
                      arr.count() == 3 ? 255 : arr.at(3).toInt());
    }
}
