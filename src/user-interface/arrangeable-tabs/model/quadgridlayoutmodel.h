#ifndef QUADGRIDLAYOUTMODEL_H
#define QUADGRIDLAYOUTMODEL_H

#include "user-interface_global.h"
#include <QObject>
#include "quadgridlayoutdefs.h"
#include <QHash>
#include "quadgridlayoutpoint.h"

class QWidget;

namespace UserInterface
{
    class QuadGridLayoutModel : public QObject
    {
        Q_OBJECT
    public:
        explicit QuadGridLayoutModel(QObject *parent = nullptr);

        void clear();

    signals:

    public slots:

    private:
        typedef QList<QuadGridLayoutDefs::GridCell> GridCellList;
        typedef QHash<QWidget*, GridCellList> WidgetCellMap;

        QWidget* widgetAt(QuadGridLayoutDefs::GridCell cell) const;
        QWidget* widgetAt(const QuadGridLayoutPoint& point) const;

        GridCellList widgetCells(QWidget* widget) const;
        int widgetCellCount(QWidget* widget) const;
        QuadGridLayoutDefs::WidgetSpan widgetSpan(QWidget* widget) const;

        static QuadGridLayoutDefs::GridCell neighbourCell(QuadGridLayoutDefs::GridCell cell, Qt::Orientation direction);

        QWidget* m_CellToWidget[4];
        WidgetCellMap m_WidgetToCells;
    };
}

#endif // QUADGRIDLAYOUTMODEL_H
