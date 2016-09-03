#include "jsonutil.h"

namespace NS_UTIL
{
    namespace Json
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

        QString jsonArrayToFlagsString(const QJsonArray &arr)
        {
            QString s;
            bool hasFlag = false;

            for ( int i = 0; i < arr.count(); i++ )
            {
                if ( hasFlag )
                {
                    s += "|";
                }

                hasFlag = true;
                s += arr.at(i).toString();
            }

            return s;
        }
    }
}
