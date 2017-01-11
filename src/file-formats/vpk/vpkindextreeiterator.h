#ifndef VPKINDEXTREEITERATOR_H
#define VPKINDEXTREEITERATOR_H

#include "file-formats_global.h"
#include <QByteArray>
#include <QDataStream>
#include "vpkindextreeitem.h"
#include <QStack>
#include <QScopedPointer>

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
        bool readNextFilePathString(QByteArray& out);
        bool readNextFileData(VPKIndexTreeItem& out);
        bool readNextFilePathOntoStack();
        void invalidate();
        QString filePathFromStack() const;
        void tryReadAtCurrentPosition();

        QByteArray* m_pTreeData;
        QScopedPointer<QDataStream> m_pDataStream;
        VPKIndexTreeItem m_TreeItem;
        QStack<QByteArray> m_FilePathStack;
    };
}

#endif // VPKINDEXTREEITERATOR_H
