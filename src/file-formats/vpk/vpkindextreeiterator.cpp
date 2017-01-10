#include "vpkindextreeiterator.h"

namespace FileFormats
{
    VPKIndexTreeIterator::VPKIndexTreeIterator(QByteArray *treeData)
        : m_pTreeData(treeData),
          m_iNextIndex(0)
    {
    }

    bool VPKIndexTreeIterator::isValid() const
    {
        return m_pTreeData;
    }

    QString VPKIndexTreeIterator::filePath() const
    {
        // TODO
        return QString();
    }

    VPKIndexTreeIterator& VPKIndexTreeIterator::advance()
    {
        // TODO
        return *this;
    }

    VPKIndexTreeIterator& VPKIndexTreeIterator::reset()
    {
        // TODO
        return *this;
    }
}
