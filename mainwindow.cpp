#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGuiApplication>
#include "application.h"
#include <QtDebug>
#include "scene.h"
#include "scenecamera.h"
#include "originmarker.h"
#include "inputprocessor.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_iActiveDocument = -1;

    setUpConnections();

    updateDocumentList(QList<MapDocument*>());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::quit()
{
    qApp->quit();
}

void MainWindow::updateDocumentList(const QList<MapDocument *> &docs)
{
    ui->menuDocuments->clear();

    if ( docs.count() < 1 )
    {
        ui->menuDocuments->addAction("No documents")->setEnabled(false);
        return;
    }

    for (int i = 0; i < docs.count(); i++)
    {
        MapDocument* doc = docs.at(i);
        QString text = QString("%0%1").arg(doc->objectName()).arg(i == m_iActiveDocument ? " [Active]" : "");
        QAction* a = ui->menuDocuments->addAction(text);
        a->setProperty("documentIndex", QVariant(i));
        a->connect(a, SIGNAL(triggered(bool)), this, SLOT(makeDocumentActiveFromMenu()));
    }
}

void MainWindow::createNewDocument()
{
    MapDocument* oldDoc = activeDocument();
    m_iActiveDocument = application()->documentCount();
    application()->newDocument();
    changeActiveDocument(oldDoc, activeDocument());
}

void MainWindow::makeDocumentActiveFromMenu()
{
    QAction* action = qobject_cast<QAction*>(sender());
    Q_ASSERT(action);

    bool ok = false;
    int index = action->property("documentIndex").toInt(&ok);
    Q_ASSERT(ok);

    MapDocument* oldDoc = activeDocument();
    m_iActiveDocument = index;
    updateDocumentList(application()->documents());
    changeActiveDocument(oldDoc, activeDocument());
}

void MainWindow::closeActiveDocument()
{
    if ( m_iActiveDocument < 0 ) return;

    MapDocument* oldDoc = activeDocument();

    int old = m_iActiveDocument;
    if ( m_iActiveDocument == application()->documentCount() - 1 )
    {
        m_iActiveDocument--;
    }

    changeActiveDocument(oldDoc, activeDocument());
    application()->closeDocument(old);
}

void MainWindow::changeActiveDocument(MapDocument *oldDoc, MapDocument *newDoc)
{
    if ( oldDoc )
    {
        ui->viewport->removeEventFilter(oldDoc->inputProcessor());
    }

    if ( newDoc )
    {
        ui->viewport->setBackgroundColor(newDoc->backgroundColor());

        QList<SceneCamera*> cameras = newDoc->scene()->findCameras();
        if ( cameras.count() > 0 )
        {
            ui->viewport->setCamera(cameras.at(0));
            ui->viewport->setScene(newDoc->scene());
        }

        ui->viewport->installEventFilter(newDoc->inputProcessor());

        populateSceneTree(newDoc->scene());
    }
    else
    {
        ui->viewport->setCamera(NULL);

        ui->sceneTreeWidget->clear();
    }
}

MapDocument* MainWindow::activeDocument() const
{
    Q_ASSERT(m_iActiveDocument < application()->documentCount());

    return m_iActiveDocument < 0 ? NULL : application()->document(m_iActiveDocument);
}

void MainWindow::setUpConnections()
{
    ui->actionScene_Tree->setProperty("linkedDockWidget", QVariant::fromValue<QObject*>(ui->dockSceneTree));
}

void MainWindow::changeDockWidgetVisibility(bool visible)
{
    QAction* action = qobject_cast<QAction*>(sender());
    Q_ASSERT(action);

    QDockWidget* widget = qobject_cast<QDockWidget*>(action->property("linkedDockWidget").value<QObject*>());
    Q_ASSERT(widget);

    widget->blockSignals(true);
    widget->setVisible(visible);
    widget->blockSignals(false);
}

void MainWindow::populateSceneTree(Scene *scene)
{
    ui->sceneTreeWidget->clear();
    QList<QTreeWidgetItem*> items;
    populateSceneTreeRecursive(scene->root(), NULL, items);

    foreach ( QTreeWidgetItem* item, items )
    {
        if ( !item->parent() )
        {
            ui->sceneTreeWidget->addTopLevelItem(item);
        }

        item->setExpanded(true);
    }
}

void MainWindow::populateSceneTreeRecursive(SceneObject *object, QTreeWidgetItem* parent, QList<QTreeWidgetItem*> &items)
{
    QStringList strings;
    QString name = object->objectName().trimmed();
    if ( name.isNull() || name.isEmpty() )
        name = QString("Unnamed node");

    strings.append(object->objectName());
    QTreeWidgetItem* i = new QTreeWidgetItem(parent, strings);
    i->setData(0, Qt::UserRole, QVariant::fromValue<QObject*>(object));
    if ( !object->editable() )
        i->setDisabled(true);

    QString iconPath(":/icons/tree_object.png");
    if ( qobject_cast<SceneCamera*>(object) )
    {
        iconPath = QString(":/icons/tree_camera.png");
    }
    else if ( qobject_cast<OriginMarker*>(object) )
    {
        iconPath = QString(":/icons/tree_origin.png");
    }

    i->setIcon(0, QIcon(QPixmap::fromImage(QImage(iconPath))));
    items.append(i);

    QList<SceneObject*> children = object->children();
    foreach ( SceneObject* o, children )
    {
        populateSceneTreeRecursive(o, i, items);
    }
}

void MainWindow::sceneTreeItemDoubleClicked(QTreeWidgetItem* item, int column)
{
    if ( !ui->viewport->scene() || !ui->viewport->camera() )
        return;

    QVariant v = item->data(column, Qt::UserRole);
    if ( v.isNull() )
        return;

    QObject* object = v.value<QObject*>();
    Q_ASSERT(object);

    SceneObject* sceneObject = qobject_cast<SceneObject*>(object);
    Q_ASSERT(sceneObject);

    QVector3D pos = sceneObject->position();
    SceneCamera* c = ui->viewport->camera();
    if ( qobject_cast<SceneCamera*>(sceneObject) == c )
        return;

    c->setPosition(pos + QVector3D(64,0,0));
    c->lookAt(pos);
    ui->viewport->update();
}

void MainWindow::sceneTreeItemClicked(QTreeWidgetItem *item, int column)
{
    MapDocument* doc = activeDocument();
    if ( !doc )
        return;

    QVariant v = item->data(column, Qt::UserRole);
    if ( v.isNull() )
        return;

    QObject* object = v.value<QObject*>();
    Q_ASSERT(object);

    SceneObject* sceneObject = qobject_cast<SceneObject*>(object);
    Q_ASSERT(sceneObject);

    doc->selectedSetClear();
    doc->selectedSetInsert(sceneObject);
    ui->viewport->update();
}
