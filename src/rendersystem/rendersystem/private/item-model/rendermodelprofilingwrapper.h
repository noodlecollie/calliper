#ifndef RENDERMODELPROFILINGWRAPPER_H
#define RENDERMODELPROFILINGWRAPPER_H

#include <QAbstractItemModel>

class RenderModelProfilingWrapper : public QAbstractItemModel
{
    Q_OBJECT
public:
    RenderModelProfilingWrapper(QObject* parent = Q_NULLPTR);

    virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    virtual QModelIndex parent(const QModelIndex &index) const override;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
    QModelIndex getIndexForTopLevelItem(int row, int column) const;

    QAbstractItemModel& m_RenderModelStoreItemModel;
};

#endif // RENDERMODELPROFILINGWRAPPER_H
