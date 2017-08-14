#ifndef PROFILERITEMMODELADATPER_H
#define PROFILERITEMMODELADATPER_H

#include <QAbstractItemModel>
#include <QVector>

namespace Profiling
{
    class ProfilerModel;

    class ProfilerItemModelAdatper : public QAbstractItemModel
    {
        Q_OBJECT
    public:
        enum Column
        {
            PlaceholderColumn = 0,

            TOTAL_COLUMNS,
        };

        ProfilerItemModelAdatper(const ProfilerModel& model, QObject* parent = Q_NULLPTR);

        void refresh();

        virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
        virtual QModelIndex parent(const QModelIndex &child) const override;
        virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;
        virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    private:
        typedef QVector<int> SlotVector;

        bool isValidSlot(int slotIndex) const;

        const ProfilerModel& m_Model;

        QVector<SlotVector> m_ChildLists;
    };
}

#endif // PROFILERITEMMODELADATPER_H
