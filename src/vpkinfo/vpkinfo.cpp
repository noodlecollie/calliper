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

void VPKInfo::print() const
{
    if ( !m_strErrorHint.isEmpty() )
    {
        qDebug() << "Error reading VPK:" << m_strErrorHint;
        return;
    }

    printHeaderData();
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
    qSetFieldWidth(18) << (_value) << qSetFieldWidth(0) << endl

    s << HEADER_FIELD("File signature", toHex<quint32>(m_Header.signature(), true));
    s << HEADER_FIELD("Version",        m_Header.version());
    s << HEADER_FIELD("Tree size",      QString("%1 bytes").arg(m_Header.signature()));

    if ( m_Header.version() != 2 )
        return;

    s << HEADER_FIELD("File data section",      QString("%1 bytes").arg(m_Header.fileDataSectionSize()));
    s << HEADER_FIELD("Archive MD5 section",    QString("%1 bytes").arg(m_Header.archiveMD5SectionSize()));
    s << HEADER_FIELD("Other MD5 section",      QString("%1 bytes").arg(m_Header.otherMD5SectionSize()));
    s << HEADER_FIELD("Signature section",      QString("%1 bytes").arg(m_Header.signatureSectionSize()));

#undef HEADER_FIELD

    s.flush();
    qDebug() << string.toLatin1().constData();
}
