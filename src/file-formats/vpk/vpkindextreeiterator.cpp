#include "vpkindextreeiterator.h"
#include "string/stringutil.h"
#include <QtDebug>

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
          m_pDataStream(nullptr),
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

    QString VPKIndexTreeIterator::filePath() const
    {
        if ( !isValid() )
            return QString();

        return filePathFromStack();
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

        // This is just for advancing the stream the correct amount.
        // Because the item hasn't been cleared, there's no guarantee
        // the data will be correct.
        readNextFileData(m_TreeItem);
    }

    VPKIndexTreeIterator& VPKIndexTreeIterator::reset()
    {
        if ( m_pTreeData )
        {
            m_pDataStream.reset(new QDataStream(*m_pTreeData));
            resetFilePathList();
            tryReadAtCurrentPosition();
        }

        return *this;
    }

    void VPKIndexTreeIterator::invalidate()
    {
        m_pDataStream->setStatus(QDataStream::ReadPastEnd);
    }

    bool VPKIndexTreeIterator::readNextFilePathString(QByteArray &out)
    {
        out = CalliperUtil::StringUtil::getNullTerminatedString(*m_pDataStream);
        return !out.isNull();
    }

    bool VPKIndexTreeIterator::readNextFileData(VPKIndexTreeItem &out)
    {
        return out.populate(*m_pDataStream);
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
                    if ( next.length() < 2 )
                    {
                        return false;
                    }
                    else
                    {
                        m_FilePathList.replace(0, next);
                        incrementTargetDepth();
                    }
                } break;

                case FILE_PATH:
                {
                    if ( next.length() < 2 )
                    {
                        m_FilePathList.replace(1, QByteArray());
                        decrementTargetDepth();
                    }
                    else
                    {
                        m_FilePathList.replace(1, next.trimmed());
                        incrementTargetDepth();
                    }
                } break;

                case FILE_NAME:
                {
                    if ( next.length() < 2 )
                    {
                        m_FilePathList.replace(2, QByteArray());
                        decrementTargetDepth();
                    }
                    else
                    {
                        m_FilePathList.replace(2, next);
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
