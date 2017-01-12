#ifndef VPKINDEXTREEITERATOR_H
#define VPKINDEXTREEITERATOR_H

#include "file-formats_global.h"
#include <QByteArray>
#include <QDataStream>
#include "vpkindextreeitem.h"
#include <QList>
#include <QScopedPointer>

// Things to note:
//
// - Valve are inconsistent (this is becoming my life mantra)
// - Just because a path exists in the index doesn't mean a filename does!
//   For example, tf2_misc contains: png->resource/econ/generated_icons/largetest->null,
//   which evaluates to resource/econ/generated_icons/largetest/[nothing].png.
//   In a sensible world this would be invalid, but see the previous point.
// - If no file path applies (eg for things in the root directory), the file path is
//   substituted with a space.

namespace FileFormats
{
    class FILEFORMATSSHARED_EXPORT VPKIndexTreeIterator
    {
    public:
        explicit VPKIndexTreeIterator(QByteArray* treeData);

        bool isValid() const;
        VPKIndexTreeIterator& advance();
        VPKIndexTreeIterator& reset();
        QString errorHint() const;

        QString fullPath() const;
        QString path() const;
        QString fileName() const;
        QString extension() const;
        const VPKIndexTreeItem& treeItem() const;

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
        bool readNextFileData();
        bool readNextFilePathOntoStack();
        void invalidate();
        QString filePathFromStack() const;
        bool hasPathOnStack() const;
        void tryReadAtCurrentPosition();
        void resetFilePathList();
        void incrementTargetDepth();
        void decrementTargetDepth();
        void advanceDepth();

        QByteArray* m_pTreeData;
        QScopedPointer<QDataStream> m_pDataStream;
        VPKIndexTreeItem m_TreeItem;
        QString m_strErrorHint;

        int m_iCurrentPathDepth;
        int m_iTargetPathDepth;
        QList<QByteArray> m_FilePathList;
    };
}

#endif // VPKINDEXTREEITERATOR_H
