#include "vpkinfo.h"
#include "vpk/vpkheader.h"
#include <QtDebug>
#include <QTextStream>

namespace
{
    template<typename T>
    inline QString toHex(const T& val, bool leading0x = false)
    {
        return QString(leading0x ? "0x%1" : "%1").arg(val, sizeof(T)*2, 16);
    }
}

VPKInfo::VPKInfo()
{

}

void VPKInfo::read(QDataStream &in)
{
    if ( !m_Header.populate(in, &m_strErrorHint) )
    {
        m_strErrorHint = "Header error: " + m_strErrorHint;
        return;
    }
}

QString VPKInfo::errorString() const
{
    return m_strErrorHint;
}

void VPKInfo::printHeaderData() const
{

    QString string;
    QTextStream s(&string);

    s << "======================================\n"
      << "=               Header               =\n"
      << "======================================\n";

#define HEADER_FIELD(_key, _value) \
    qSetFieldWidth(21) << QString("%1:").arg(_key).toLatin1().constData() <<\
    qSetFieldWidth(18) << (_value) << qSetFieldWidth(0)

    s << HEADER_FIELD("File signature", toHex<quint32>(m_Header.signature(), true)) << endl;
    s << HEADER_FIELD("Version",        m_Header.version()) << endl;
    s << HEADER_FIELD("Tree size",      QString("%1 bytes").arg(m_Header.treeSize()))
      << QString(" (%1 KB)").arg(static_cast<float>(m_Header.treeSize()) / 1024.0f) << endl;

    if ( m_Header.version() != 2 )
        return;

    s << HEADER_FIELD("File data section",      QString("%1 bytes").arg(m_Header.fileDataSectionSize())) << endl;
    s << HEADER_FIELD("Archive MD5 section",    QString("%1 bytes").arg(m_Header.archiveMD5SectionSize())) << endl;
    s << HEADER_FIELD("Other MD5 section",      QString("%1 bytes").arg(m_Header.otherMD5SectionSize())) << endl;
    s << HEADER_FIELD("Signature section",      QString("%1 bytes").arg(m_Header.signatureSectionSize())) << endl;

#undef HEADER_FIELD

    s.flush();
    qDebug() << string.toLatin1().constData();
}
