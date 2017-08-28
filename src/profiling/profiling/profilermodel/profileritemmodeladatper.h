#ifndef PROFILERITEMMODELADATPER_H
#define PROFILERITEMMODELADATPER_H

#include "profiling_global.h"

#include <QAbstractItemModel>
#include <QVector>
#include <QTimer>

namespace Profiling
{
    class ProfilerModel;

    class PROFILINGSHARED_EXPORT ProfilerItemModelAdatper : public QAbstractItemModel
    {
        Q_OBJECT
    public:
        enum Column
        {
            PlaceholderColumn = 0,

            TOTAL_COLUMNS,
        };

        ProfilerItemModelAdatper(const ProfilerModel& model, QObject* parent = Q_NULLPTR);
        virtual ~ProfilerItemModelAdatper();

        virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
        virtual QModelIndex parent(const QModelIndex &child) const override;
        virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;
        virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

        int updateFrequency() const;
        void setUpdateFrequency(int msec);

    private slots:
        void flagNeedsUpdate();
        void requestUpdate();

    private:
        typedef QVector<int> SlotVector;

        void refresh();
        bool isValidSlot(int slotIndex) const;
        const SlotVector& childrenOfParent(int parentIndex) const;
        QModelIndex createIndexForSlot(int slotIndexInProfilerModel, int slotIndexInParentsChildrenList, int column = 0) const;

        const ProfilerModel& m_Model;

        QVector<SlotVector> m_ChildLists;
        QTimer m_UpdateTimer;
        bool m_bNeedsUpdate;
    };
}

#endif // PROFILERITEMMODELADATPER_H
