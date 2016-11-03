#ifndef VPKPARSER_H
#define VPKPARSER_H

#include "file-formats_global.h"
#include "vpkindex.h"
#include <QFile>
#include "vpkheader.h"

namespace FileFormats
{
    class VPKParser
    {
    public:
        VPKParser(const QString& filename);

        bool open();
        void close();
        bool isOpen() const;

        bool createIndex(VPKIndex& index, QString* errorHint = nullptr);

    private:
        bool populateIndex(VPKIndex& index, QDataStream& stream, QString* errorHint);
        bool createRecord(VPKIndex& index, QDataStream& stream, QString* errorHint);

        QFile m_File;
        VPKHeader m_Header;

        QString m_strCurrentExtension;
        QString m_strCurrentPath;
        QString m_strCurrentFilename;
    };
}

#endif // VPKPARSER_H
