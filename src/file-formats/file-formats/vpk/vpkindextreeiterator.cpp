#include "vpkindextreeiterator.h"
#include "calliperutil/string/stringutil.h"
#include <QtDebug>
#include "calliperutil/general/paths.h"

namespace
{
    enum
    {
        FILE_EXTENSION = 0,
        FILE_PATH,
        FILE_NAME,

        TOTAL_SEGMENTS
    };
}

namespace FileFormats
{
    VPKIndexTreeIterator::VPKIndexTreeIterator(QByteArray *treeData)
        : m_pTreeData(treeData),
          m_pDataStream(Q_NULLPTR),
          m_iCurrentPathDepth(0),
          m_iTargetPathDepth(0)
    {
        if ( treeData )
        {
            reset();
        }
        else
        {
            m_pDataStream.reset(new QDataStream());
            invalidate();
        }
    }

    bool VPKIndexTreeIterator::isValid() const
    {
        return m_pTreeData && m_pDataStream->status() == QDataStream::Ok;
    }

    QString VPKIndexTreeIterator::fullPath() const
    {
        if ( !isValid() )
            return QString();

        return filePathFromStack();
    }

    QString VPKIndexTreeIterator::path() const
    {
        if ( !isValid() )
            return QString();

        return m_FilePathList.at(FILE_PATH);
    }

    QString VPKIndexTreeIterator::fileName() const
    {
        if ( !isValid() )
            return QString();

        return m_FilePathList.at(FILE_NAME);
    }

    QString VPKIndexTreeIterator::extension() const
    {
        if ( !isValid() )
            return QString();

        return m_FilePathList.at(FILE_EXTENSION);
    }

    const VPKIndexTreeItem& VPKIndexTreeIterator::treeItem() const
    {
        return m_TreeItem;
    }

    QString VPKIndexTreeIterator::errorHint() const
    {
        if ( hasPathOnStack() )
        {
            return QString("When processing file %1: %2")
                    .arg(filePathFromStack())
                    .arg(m_strErrorHint);
        }

        return m_strErrorHint;
    }

    VPKIndexTreeIterator& VPKIndexTreeIterator::advance()
    {
        if ( !isValid() )
            return *this;

        tryReadAtCurrentPosition();

        return *this;
    }

    void VPKIndexTreeIterator::tryReadAtCurrentPosition()
    {
        if ( !readNextFilePathOntoStack() )
        {
            invalidate();
            return;
        }

        // Make sure the file data is accurate too.
        if ( !readNextFileData() )
        {
            invalidate();
            return;
        }
    }

    VPKIndexTreeIterator& VPKIndexTreeIterator::reset()
    {
        if ( m_pTreeData )
        {
            m_pDataStream.reset(new QDataStream(*m_pTreeData));
            m_TreeItem.clear();
            m_strErrorHint = QString();
            resetFilePathList();
            tryReadAtCurrentPosition();
        }

        return *this;
    }

    void VPKIndexTreeIterator::invalidate()
    {
        m_pDataStream->setStatus(QDataStream::ReadPastEnd);
        m_TreeItem.clear();
    }

    bool VPKIndexTreeIterator::readNextFilePathString(QByteArray &out)
    {
        out = CalliperUtil::StringUtil::getNullTerminatedString(*m_pDataStream);
        return !out.isNull();
    }

    bool VPKIndexTreeIterator::readNextFileData()
    {
        return m_TreeItem.populate(*m_pDataStream, &m_strErrorHint);
    }

    bool VPKIndexTreeIterator::readNextFilePathOntoStack()
    {
        while (true)
        {
            QByteArray next;
            if ( !readNextFilePathString(next) )
            {
                return false;
            }

            Q_ASSERT_X(!next.isNull(), Q_FUNC_INFO, "Expected a valid byte array!");

            switch ( m_iCurrentPathDepth )
            {
                case FILE_EXTENSION:
                {
                    if ( next.isEmpty() )
                    {
                        m_FilePathList.replace(FILE_EXTENSION, QByteArray());
                        return false;
                    }
                    else
                    {
                        m_FilePathList.replace(FILE_EXTENSION, next);
                        incrementTargetDepth();
                    }
                } break;

                case FILE_PATH:
                {
                    if ( next.isEmpty() )
                    {
                        m_FilePathList.replace(FILE_PATH, QByteArray());
                        decrementTargetDepth();
                    }
                    else
                    {
                        m_FilePathList.replace(FILE_PATH, CalliperUtil::General::normaliseResourcePathSeparators(next.trimmed()));
                        incrementTargetDepth();
                    }
                } break;

                case FILE_NAME:
                {
                    if ( next.isEmpty() )
                    {
                        m_FilePathList.replace(FILE_NAME, QByteArray());
                        decrementTargetDepth();
                    }
                    else
                    {
                        m_FilePathList.replace(FILE_NAME, next);
                        return true;
                    }
                } break;
            }

            advanceDepth();
        }
    }

    QString VPKIndexTreeIterator::filePathFromStack() const
    {
        const QByteArray& path = m_FilePathList.at(FILE_PATH);
        const QByteArray& name = m_FilePathList.at(FILE_NAME);
        const QByteArray& ext = m_FilePathList.at(FILE_EXTENSION);

        return QString("%1%2%3.%4")
                .arg(path.constData())
                .arg(path.isEmpty() ? "" : "/")
                .arg(name.constData())
                .arg(ext.constData());
    }

    bool VPKIndexTreeIterator::hasPathOnStack() const
    {
        foreach ( const QByteArray& arr, m_FilePathList )
        {
            if ( !arr.isNull() )
                return true;
        }

        return false;
    }

    void VPKIndexTreeIterator::resetFilePathList()
    {
        m_FilePathList.clear();
        for ( int i = 0; i < TOTAL_SEGMENTS; ++i )
        {
            m_FilePathList.append(QByteArray());
        }

        m_iCurrentPathDepth = 0;
        m_iTargetPathDepth = 0;
    }

    void VPKIndexTreeIterator::incrementTargetDepth()
    {
        ++m_iTargetPathDepth;

        Q_ASSERT_X(m_iTargetPathDepth >= FILE_EXTENSION && m_iTargetPathDepth < TOTAL_SEGMENTS,
                   Q_FUNC_INFO,
                   "Target depth out of range!");
    }

    void VPKIndexTreeIterator::decrementTargetDepth()
    {
        --m_iTargetPathDepth;

        Q_ASSERT_X(m_iTargetPathDepth >= FILE_EXTENSION && m_iTargetPathDepth < TOTAL_SEGMENTS,
                   Q_FUNC_INFO,
                   "Target depth out of range!");
    }

    void VPKIndexTreeIterator::advanceDepth()
    {
        m_iCurrentPathDepth = m_iTargetPathDepth;
    }
}
