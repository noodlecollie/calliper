#ifndef VPKINDEXTREEITERATOR_H
#define VPKINDEXTREEITERATOR_H

#include "file-formats_global.h"
#include <QByteArray>

namespace FileFormats
{
    class VPKIndexTreeIterator
    {
    public:
        explicit VPKIndexTreeIterator(QByteArray* treeData);

        bool isValid() const;
        QString filePath() const;
        VPKIndexTreeIterator& advance();
        VPKIndexTreeIterator& reset();

        inline VPKIndexTreeIterator& operator ++()
        {
            return advance();
        }

        inline QString operator *() const
        {
            return QString();
        }

    private:

        QByteArray* m_pTreeData;
    };
}

#endif // VPKINDEXTREEITERATOR_H
