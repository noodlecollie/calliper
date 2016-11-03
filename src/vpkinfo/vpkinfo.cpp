#include "vpkinfo.h"
#include "vpk/vpkheader.h"
#include <QtDebug>
#include <QTextStream>
#include "vpk/vpkfile.h"

namespace VPKInfo
{
    namespace
    {
        template<typename T>
        inline QString toHex(const T& val, bool leading0x = false)
        {
            return QString(leading0x ? "0x%1" : "%1").arg(val, sizeof(T)*2, 16);
        }
    }

    void printHeaderData(const FileFormats::VPKHeader &header)
    {

        QString string;
        QTextStream s(&string);

        s << "======================================\n"
          << "=               Header               =\n"
          << "======================================\n";

    #define HEADER_FIELD(_key, _value) \
        qSetFieldWidth(21) << QString("%1:").arg(_key).toLatin1().constData() <<\
        qSetFieldWidth(18) << (_value) << qSetFieldWidth(0)

        s << HEADER_FIELD("File signature", toHex<quint32>(header.signature(), true)) << endl;
        s << HEADER_FIELD("Version",        header.version()) << endl;
        s << HEADER_FIELD("Tree size",      QString("%1 bytes").arg(header.treeSize()))
          << QString(" (%1 KB)").arg(static_cast<float>(header.treeSize()) / 1024.0f) << endl;

        if ( header.version() != 2 )
            return;

        s << HEADER_FIELD("File data section",      QString("%1 bytes").arg(header.fileDataSectionSize())) << endl;
        s << HEADER_FIELD("Archive MD5 section",    QString("%1 bytes").arg(header.archiveMD5SectionSize())) << endl;
        s << HEADER_FIELD("Other MD5 section",      QString("%1 bytes").arg(header.otherMD5SectionSize())) << endl;
        s << HEADER_FIELD("Signature section",      QString("%1 bytes").arg(header.signatureSectionSize())) << endl;

    #undef HEADER_FIELD

        s.flush();
        qDebug() << string.toLatin1().constData();
    }

    void printIndexData(const FileFormats::VPKIndex &index)
    {

    }
}
