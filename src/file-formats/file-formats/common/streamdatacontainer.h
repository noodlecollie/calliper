#ifndef STREAMDATACONTAINER_H
#define STREAMDATACONTAINER_H

#include "file-formats_global.h"
#include <QDataStream>
#include <QString>

namespace FileFormats
{
    class FILEFORMATSSHARED_EXPORT StreamDataContainer
    {
    public:
        virtual ~StreamDataContainer() {}

        virtual bool populate(QDataStream& stream, QString* errorHint = Q_NULLPTR) = 0;

        void beginRead(QDataStream& stream);
        bool endRead(QDataStream& stream, QString* errorHint);

    protected:
        static void setErrorString(QString* errorString, const QString& msg);

        // "An error was encountered while reading [name] data: ..."
        virtual QString containerName() const = 0;
    };
}

#endif // STREAMDATACONTAINER_H
