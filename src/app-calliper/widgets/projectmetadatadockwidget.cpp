#include "projectmetadatadockwidget.h"
#include <QFormLayout>
#include <QLineEdit>
#include <QMetaProperty>
#include <QtDebug>

namespace
{
    QVariant getContents(QWidget* widget)
    {
        QLineEdit* lineEdit = qobject_cast<QLineEdit*>(widget);
        if ( lineEdit )
        {
            return QVariant(lineEdit->text());
        }

        return QVariant();
    }

    void setContents(QWidget* widget, const QVariant& contents)
    {
        QLineEdit* lineEdit = qobject_cast<QLineEdit*>(widget);
        if ( lineEdit )
        {
            lineEdit->setText(contents.toString());
            return;
        }
    }
}

namespace AppCalliper
{
    ProjectMetadataDockWidget::ProjectMetadataDockWidget(QWidget *parent, Qt::WindowFlags flags)
        : VisibleActionDockWidget(tr("Project Metadata"), parent, flags),
          m_pProjectMetadata(nullptr)
    {
        setupUi();
        updateUiEnabledState();
    }

    Model::CalliperProjectMetadata* ProjectMetadataDockWidget::projectMetadata() const
    {
        return m_pProjectMetadata;
    }

    void ProjectMetadataDockWidget::setProjectMetadata(Model::CalliperProjectMetadata *metadata)
    {
        if ( metadata == m_pProjectMetadata )
            return;

        transitionSignals(m_pProjectMetadata, metadata);
        m_pProjectMetadata = metadata;
        updateUiEnabledState();
    }

    void ProjectMetadataDockWidget::transitionSignals(Model::CalliperProjectMetadata *oldObj, Model::CalliperProjectMetadata *newObj)
    {
        if ( oldObj )
        {
            disconnect(oldObj, SIGNAL(dataChanged(int)), this, SLOT(propertyUpdated(int)));
        }

        if ( newObj )
        {
            connect(newObj, SIGNAL(dataChanged(int)), this, SLOT(propertyUpdated(int)));
        }
    }

    void ProjectMetadataDockWidget::setupUi()
    {
        m_pContentsWidget = new QWidget();
        QFormLayout* formLayout = new QFormLayout();

        m_pContentsWidget->setLayout(formLayout);
        setWidget(m_pContentsWidget);

        addLineEditRow(tr("Project Name"), new QLineEdit(), SIGNAL(editingFinished()), "projectName");
    }

    void ProjectMetadataDockWidget::addLineEditRow(const QString &labelText, QWidget *widget,
                                                   const char *widgetSignal, const QString& propertyName)
    {
        QFormLayout* formLayout = qobject_cast<QFormLayout*>(m_pContentsWidget->layout());
        if ( !formLayout )
            return;

        formLayout->addRow(labelText, widget);
        m_WidgetToPropertyName.insert(widget, propertyName);
        m_PropertyNameToWidget.insert(propertyName, widget);
        connect(widget, widgetSignal, this, SLOT(uiDelegateEdited()));
    }

    void ProjectMetadataDockWidget::uiDelegateEdited()
    {
        if ( !m_pProjectMetadata )
            return;

        QWidget* widget = qobject_cast<QWidget*>(sender());
        if ( !widget )
            return;

        QString propertyName = m_WidgetToPropertyName.value(widget, QString());
        if ( propertyName.isNull() )
            return;

        m_pProjectMetadata->setProperty(propertyName.toLatin1().constData(), getContents(widget));
    }

    void ProjectMetadataDockWidget::propertyUpdated(int propertyIndex)
    {
        if ( !m_pProjectMetadata || propertyIndex < 0 || propertyIndex >= m_pProjectMetadata->metaObject()->propertyCount() )
        {
            return;
        }

        QMetaProperty property = m_pProjectMetadata->metaObject()->property(propertyIndex);
        QWidget* widget = m_PropertyNameToWidget.value(property.name(), nullptr);
        if ( !widget )
            return;

        setContents(widget, property.read(m_pProjectMetadata));
    }

    void ProjectMetadataDockWidget::updateUiEnabledState()
    {
        if ( !m_pProjectMetadata && m_pContentsWidget->isEnabled() )
        {
            clearUiDelegates();
        }

        m_pContentsWidget->setEnabled(m_pProjectMetadata != nullptr);
    }

    void ProjectMetadataDockWidget::clearUiDelegates()
    {
        foreach ( QWidget* widget, m_PropertyNameToWidget.values() )
        {
            setContents(widget, QVariant());
        }
    }
}
