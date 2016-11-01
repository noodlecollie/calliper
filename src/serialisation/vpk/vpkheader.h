#ifndef VPKHEADER_H
#define VPKHEADER_H

#include "serialisation_global.h"
#include <QScopedPointer>
#include <QDataStream>

namespace NS_SERIALISATION
{
    class SERIALISATIONSHARED_EXPORT VPKHeader
    {
    public:
        VPKHeader();
        ~VPKHeader();

    bool populate(QDataStream& stream, QString* errorHint = nullptr);

    quint32 signature() const;
    quint32 version() const;

    quint32 treeSize() const;
    quint32 fileDataSectionSize() const;
    quint32 archiveMD5SectionSize() const;
    quint32 otherMD5SectionSize() const;
    quint32 signatureSectionSize() const;

    private:
        struct Data;
        Data* m_pData;
    };
}

#endif // VPKHEADER_H
