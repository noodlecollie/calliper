#ifndef VPKFILE_H
#define VPKFILE_H

#include "file-formats_global.h"
#include "vpkindex.h"
#include <QFile>
#include "vpkheader.h"
#include "vpkheader.h"
#include <QDataStream>

namespace FileFormats
{
    class FILEFORMATSSHARED_EXPORT VPKFile
    {
    public:
        VPKFile(const QString& filename);

        QStringList siblingArchives() const;

        bool read(QString* errorHint = nullptr);

        const VPKHeader& header() const;
        const VPKIndex& index() const;

    private:
        bool createIndex(QDataStream& stream, QString* errorHint);
        bool populateIndex(QDataStream& stream, QString* errorHint);
        bool createRecord(QDataStream& stream, QString* errorHint);

        QFile m_File;
        VPKHeader m_Header;
        VPKIndex m_Index;

        QString m_strCurrentExtension;
        QString m_strCurrentPath;
        QString m_strCurrentFilename;
    };
}

#endif // VPKFILE_H
