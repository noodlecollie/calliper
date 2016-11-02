#ifndef VPKINFO_H
#define VPKINFO_H

#include <QDataStream>
#include "vpk/vpkheader.h"

class VPKInfo
{
public:
    VPKInfo();

    void read(QDataStream& in);

    QString errorString() const;
    void printHeaderData() const;

private:

    QString m_strErrorHint;
    FileFormats::VPKHeader m_Header;
};

#endif // VPKINFO_H
