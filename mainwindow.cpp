#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGuiApplication>
#include "application.h"
#include <QtDebug>
#include "mapscene.h"
#include "scenecamera.h"
#include "originmarker.h"
#include "inputprocessor.h"
#include "tools.h"
#include <QFileDialog>
#include <QStandardPaths>
#include "callipermapfile.h"
#include <QMessageBox>

#define PROP_STRING_LINKED_TOOL	"linkedTool"

#ifdef QT_DEBUG
#define FILE_DIALOG_OPTIONS QFileDialog::DontUseNativeDialog
#else
#define FILE_DIALOG_OPTIONS 0
#endif

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_iActiveDocument = -1;
    m_pActiveViewport = NULL;
    m_szLastSaveDir = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    m_szLastLoadDir = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);

	ui->actionDebug_Tool->setProperty(PROP_STRING_LINKED_TOOL, QVariant(DebugTestTool::staticName()));
    ui->actionTranslate_Tool->setProperty(PROP_STRING_LINKED_TOOL, QVariant(TranslationTool::staticName()));

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
        if ( newDoc->activeToolIndex() < 0 )
            newDoc->setActiveToolIndex(0);

        ui->viewport->setBackgroundColor(newDoc->backgroundColor());
        ui->viewport->setDocument(newDoc);

        QList<SceneCamera*> cameras = newDoc->scene()->findCameras();
        if ( cameras.count() > 0 )
        {
            ui->viewport->setCamera(cameras.at(0));
        }
        else
        {
            ui->viewport->setCamera(NULL);
        }

        ui->viewport->installEventFilter(newDoc->inputProcessor());

        populateSceneTree(newDoc->scene());
    }
    else
    {
        ui->viewport->setDocument(NULL);
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

void MainWindow::populateSceneTree(MapScene *scene)
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
    if ( !ui->viewport->document() || !ui->viewport->camera() )
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

Viewport* MainWindow::activeViewport() const
{
    return m_pActiveViewport;
}

void MainWindow::viewportPreFrame(int msec)
{
    MapDocument* doc = activeDocument();
    if ( !doc )
        return;

    BaseTool* t = doc->activeTool();
    if ( !t )
        return;

    t->update(msec);
}

void MainWindow::toolButtonClicked()
{
	MapDocument* doc = activeDocument();
	if ( !doc )
		return;

	QAction* action = qobject_cast<QAction*>(sender());
	Q_ASSERT(action);

	QString toolName = action->property(PROP_STRING_LINKED_TOOL).toString();
	int toolIndex = doc->toolIndex(toolName);
	if ( toolIndex < 0 )
		return;

	doc->setActiveToolIndex(toolIndex);
	qDebug() << "Set active tool to" << toolName;
}

void MainWindow::saveDocument(MapDocument *document, const QString &filename)
{
    CalliperMapFile cmf(filename, document);

    // For now, save as the most human-readable format.
    if ( !cmf.saveToFile(CalliperMapFile::IndentedJson) )
    {
        QMessageBox::warning(this, "Error", QString("Unable to save file %0").arg(filename));
        return;
    }

    // Cache the file name within the document.
    document->setFilename(filename);

    // Cache the directory for next time.
    {
        QDir directory(filename);
        m_szLastSaveDir = directory.canonicalPath();
    }
}

void MainWindow::saveCurrentDocument()
{
    MapDocument* doc = activeDocument();
    if ( !doc )
        return;

    // If the document filename is null then it's never been set, so call saveAs().
    if ( doc->filename().isNull() )
    {
        saveCurrentDocumentAs();
        return;
    }

    // Save our file.
    saveDocument(doc, doc->filename());
}

void MainWindow::saveCurrentDocumentAs()
{
    MapDocument* doc = activeDocument();
    if ( !doc )
        return;

    QString filename = QFileDialog::getSaveFileName(this, "Save document as...", m_szLastSaveDir, "Calliper map files (*.cmf)", NULL, FILE_DIALOG_OPTIONS);
    if ( filename.isNull() )
        return;

    saveDocument(doc, filename);
}

void MainWindow::loadDocument()
{
    QString filename = QFileDialog::getOpenFileName(this, "Open document", m_szLastLoadDir, "Calliper map files (*.cmf)", NULL, FILE_DIALOG_OPTIONS);
    if ( filename.isNull() )
        return;

    MapDocument* doc = application()->newDocument();
    loadDocument(doc, filename);

    // The new document is created on the end of the list.
    // Switch to it.
    MapDocument* oldDoc = activeDocument();
    m_iActiveDocument = application()->documentCount() - 1;
    updateDocumentList(application()->documents());
    changeActiveDocument(oldDoc, activeDocument());
}

void MainWindow::loadDocument(MapDocument *document, const QString &filename)
{
    CalliperMapFile cmf(filename, document);

    // For now, read only indented JSON.
    if ( !cmf.loadFromFile(CalliperMapFile::IndentedJson) )
    {
        QMessageBox::warning(this, "Error", QString("Unable to load file %0").arg(filename));
        return;
    }

    // Cache the filename.
    document->setFilename(filename);

    // Cache the directory for next time.
    {
        QDir directory(filename);
        m_szLastLoadDir = directory.canonicalPath();
    }
}
