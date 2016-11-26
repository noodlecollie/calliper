#include "streamdatacontainer.h"

namespace FileFormats
{
    namespace
    {
        QString streamStatusString(QDataStream::Status status)
        {
            switch ( status )
            {
            case QDataStream::Ok:
                return "Stream is OK";

            case QDataStream::ReadPastEnd:
                return "Read past end of stream";

            case QDataStream::ReadCorruptData:
                return "Read corrupt data from stream";

            case QDataStream::WriteFailed:
                return "Writing to stream failed";

            default:
                return QString("Unknown stream status code %1")
                        .arg(status);
            }
        }
    }

    void StreamDataContainer::setErrorString(QString *errorString, const QString &msg)
    {
        if ( errorString )
            *errorString = msg;
    }

    void StreamDataContainer::beginRead(QDataStream &stream)
    {
        stream.startTransaction();
    }

    bool StreamDataContainer::endRead(QDataStream &stream, QString* errorHint)
    {
        if ( stream.status() != QDataStream::Ok )
        {
            stream.abortTransaction();
            setErrorString(errorHint, QString("An error was encountered while reading %1 data: %2.")
                           .arg(containerName())
                           .arg(streamStatusString(stream.status())));
            return false;
        }

        bool success = stream.commitTransaction();
        Q_ASSERT_X(success, Q_FUNC_INFO, "Stream status was OK but committing transaction failed!");
        return success;
    }
}
