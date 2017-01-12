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
        {
            invalidate();
            return;
        }

        readNextFileData(m_TreeItem);
        qDebug() << "Next file path:" << filePathFromStack();
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
        static int calls = 0;
        if ( calls > 5 )
            return false;

        while (true)
        {
            QByteArray next;
            if ( !readNextFilePathString(next) )
            {
                return false;
            }

            Q_ASSERT_X(next.length() > 0, Q_FUNC_INFO, "Expected a valid byte array!");

            switch ( m_FilePathStack.size() )
            {
                case 0:
                {
                    // Get next string.
                    // If string is valid, push and loop.
                    // Else return false.

                    if ( next.length() == 1 )
                    {
                        qDebug() << "No more data, invalidating";
                        return false;
                    }

                    qDebug() << "Pushing" << next << "onto level 0";
                    m_FilePathStack.push(next);
                    break;
                }

                case 1:
                {
                    // Get next string.
                    // If string is valid, push and loop.
                    // Else pop and loop.

                    if ( next.length() == 1 )
                    {
                        qDebug() << "Popping" << m_FilePathStack.top() << "from level 1";
                        m_FilePathStack.pop();
                        break;
                    }
                    else
                    {
                        qDebug() << "Pushing" << next << "onto level 1";
                        m_FilePathStack.push(next);
                        break;
                    }
                }

                case 2:
                {
                    // Get next string.
                    // If string is valid, push and return true.
                    // Else pop and loop.

                    if ( next.length() == 1 )
                    {
                        qDebug() << "Popping" << m_FilePathStack.top() << "from level 2";
                        m_FilePathStack.pop();
                        break;
                    }
                    else
                    {
                        qDebug() << "Pushing" << next << "onto level 2";
                        m_FilePathStack.push(next);
                        calls++;
                        return true;
                    }
                }

                case 3:
                {
                    // Pop and loop again.
                    // We'll only be here if we're continuing
                    // on from a previous search.
                    qDebug() << "Popping" << m_FilePathStack.top() << "from level 3";
                    m_FilePathStack.pop();
                    break;
                }

                default:
                {
                    Q_ASSERT_X(false, Q_FUNC_INFO, "Unexpected stack depth!");
                    return false;
                }
            }
        }
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
