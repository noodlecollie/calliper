#include "stringutil.h"
#include <QBuffer>

namespace CalliperUtil
{
    namespace StringUtil
    {
        QByteArray getNullTerminatedString(const QDataStream &dataStream)
        {
            if ( dataStream.status() != QDataStream::Ok )
                return QByteArray();

            QBuffer* buffer = qobject_cast<QBuffer*>(dataStream.device());
            if ( !buffer )
                return QByteArray();

            qint64 start = buffer->pos();
            if ( start < 0 || start >= arr.length() )
                return QByteArray();

            QByteArray& arr = buffer->buffer();
            int indexOfNull = arr.indexOf('\0', start);

            if ( indexOfNull < 0 )
                return QByteArray();

            return arr.mid(start, indexOfNull - start);
        }
    }
}
