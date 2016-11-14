#ifndef SIMPLEITEMCOLLECTION_H
#define SIMPLEITEMCOLLECTION_H

#include "file-formats_global.h"
#include <QList>
#include <QSharedPointer>

namespace FileFormats
{
    template<typename T>
    class SimpleItemCollection
    {
    public:
        SimpleItemCollection()
        {
        }

        inline void addItem(const QSharedPointer<T>& item)
        {
            m_Items.append(item);
        }

        inline QSharedPointer<T> itemAt(int index) const
        {
            return m_Items.at(index);
        }

        inline int itemCount() const
        {
            return m_Items.count();
        }

        inline void clearItems()
        {
            m_Items.clear();
        }

    private:
        QList<QSharedPointer<T> > m_Items;
    };
}

#endif // SIMPLEITEMCOLLECTION_H
