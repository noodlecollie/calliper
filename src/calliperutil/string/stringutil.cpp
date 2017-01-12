#include "stringutil.h"
#include <QBuffer>

namespace CalliperUtil
{
    namespace StringUtil
    {
        QByteArray getNullTerminatedString(QDataStream &dataStream)
        {
            if ( dataStream.status() != QDataStream::Ok )
                return QByteArray();

            QBuffer* buffer = qobject_cast<QBuffer*>(dataStream.device());
            if ( !buffer )
                return QByteArray();

            qint64 start = buffer->pos();
            QByteArray& arr = buffer->buffer();
            if ( start < 0 || start >= arr.length() )
                return QByteArray();

            int indexOfNull = arr.indexOf('\0', start);

            if ( indexOfNull < 0 )
                return QByteArray();

            int length = indexOfNull - start;
            dataStream.skipRawData(length+1);
            return arr.mid(start, length);
        }
    }
}
