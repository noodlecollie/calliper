#ifndef VPKINFO_H
#define VPKINFO_H

#include <QDataStream>
#include "vpk/vpkheader.h"

class VPKInfo
{
public:
    VPKInfo();

    void read(QDataStream& in);
    void print() const;

private:
    void printHeaderData() const;

    QString m_strErrorHint;
    FileFormats::VPKHeader m_Header;
};

#endif // VPKINFO_H
