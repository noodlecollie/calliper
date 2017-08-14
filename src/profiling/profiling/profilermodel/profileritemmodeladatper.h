#ifndef PROFILERITEMMODELADATPER_H
#define PROFILERITEMMODELADATPER_H

#include <QAbstractItemModel>

class ProfilerModel;

class ProfilerItemModelAdatper : public QAbstractItemModel
{
    Q_OBJECT
public:
    ProfilerItemModelAdatper(const ProfilerModel& model, QObject* parent = Q_NULLPTR);

    virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    virtual QModelIndex parent(const QModelIndex &child) const override;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
    const ProfilerModel& m_Model;
};

#endif // PROFILERITEMMODELADATPER_H
