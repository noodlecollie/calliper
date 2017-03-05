#ifndef RESIZEABLEGRIDLAYOUTCONTAINER_H
#define RESIZEABLEGRIDLAYOUTCONTAINER_H

#include <QFrame>

class QVBoxLayout;
class QHBoxLayout;
class QStackedWidget;

#include "user-interface_global.h"

namespace UserInterface
{
    class ResizeableGridLayoutContainerButton;

    class USERINTERFACESHARED_EXPORT ResizeableGridLayoutContainer : public QFrame
    {
        Q_OBJECT
    public:
        explicit ResizeableGridLayoutContainer(QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());

        void addWidget(QWidget* widget);
        QWidget* replaceWidget(int index, QWidget* widget);
        QWidget* removeWidget(int index);
        QWidget* removeCurrentWidget();

        QWidget* widgetAt(int index) const;
        int widgetCount() const;
        int currentWidgetIndex() const;
        void setCurrentWidgetIndex(int index);

    signals:
        void currentChanged(int index);
        void maximizeInvoked(int index);
        void floatInvoked(int index, bool dragged);
        void closeInvoked(int index);

    public slots:

    private slots:
        void buttonPressed(int index);

    private:
        ResizeableGridLayoutContainerButton* createButton(QWidget* widget, int index);
        void initLayout();
        ResizeableGridLayoutContainerButton* button(int index) const;

        QVBoxLayout* m_pGeneralLayout;
        QHBoxLayout* m_pButtonLayout;
        QStackedWidget* m_pStackedWidget;
    };
}

#endif // RESIZEABLEGRIDLAYOUTCONTAINER_H
