#include "vpkindextreeiterator.h"
#include "string/stringutil.h"
#include <QtDebug>

namespace FileFormats
{
    VPKIndexTreeIterator::VPKIndexTreeIterator(QByteArray *treeData)
        : m_pTreeData(treeData),
          m_pDataStream(nullptr)
    {
        if ( treeData )
        {
            m_pDataStream.reset(new QDataStream(*m_pTreeData));
            tryReadAtCurrentPosition();
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
            invalidate();
    }

    VPKIndexTreeIterator& VPKIndexTreeIterator::reset()
    {
        if ( m_pTreeData )
        {
            m_pDataStream.reset(new QDataStream(*m_pTreeData));
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
        return out.length() > 0;
    }

    bool VPKIndexTreeIterator::readNextFileData(VPKIndexTreeItem &out)
    {
        out.clear();
        return out.populate(*m_pDataStream);
    }

    bool VPKIndexTreeIterator::readNextFilePathOntoStack()
    {
        bool shouldSwap = m_FilePathStack.size() == 3;

        do
        {
            QByteArray nextChunk;
            if ( !readNextFilePathString(nextChunk) )
            {
                return false;
            }

            Q_ASSERT_X(nextChunk.length() > 0, Q_FUNC_INFO, "Expected non-zero byte array length!");

            if ( nextChunk.length() > 1 )
            {
                if ( shouldSwap )
                    m_FilePathStack.top() = nextChunk;
                else
                    m_FilePathStack.push(nextChunk);

                shouldSwap = false;

                if ( m_FilePathStack.size() == 3 )
                {
                    if ( !readNextFileData(m_TreeItem) )
                    {
                        return false;
                    }
                }
            }
            else
            {
                if ( m_FilePathStack.size() < 1 )
                {
                    return false;
                }
                else
                {
                    m_FilePathStack.pop();
                    shouldSwap = true;
                }
            }
        }
        while ( m_FilePathStack.size() < 3 );

        qDebug() << "VPK Index Tree Iterator: Got next file path:" << filePathFromStack();
        return true;
    }

    QString VPKIndexTreeIterator::filePathFromStack() const
    {
        if ( m_FilePathStack.size() != 3 )
            return QString();

        QList<QByteArray> list = m_FilePathStack.toList();
        return QString("%1/%2.%3")
                .arg(list.at(1).constData())
                .arg(list.at(2).constData())
                .arg(list.at(0).constData());
    }
}
