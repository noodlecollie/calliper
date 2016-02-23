#include "mapdocument.h"
#include "mapscene.h"
#include "scenefactory.h"
#include "sceneobject.h"
#include "tools.h"
#include "inputprocessor.h"
#include "openglrenderer.h"
#include "jsonutil.h"

#define DEBUG_SCENES

MapDocument::MapDocument(QObject *parent) : QObject(parent)
{

#ifdef DEBUG_SCENES
    m_pScene = SceneFactory::debugScene(this);
    m_pUIScene = SceneFactory::defaultUIScene(this);
#else
    m_pScene = SceneFactory::defaultScene(this);
    m_pUIScene = SceneFactory::defaultUIScene(this);
#endif

    m_iFileFormat = CalliperMapFile::IndentedJson;

    m_colBackground = QColor::fromRgb(0xff262626);
    m_colSelected = QColor::fromRgb(0xffff0000);

    m_pInputProcessor = new InputProcessor(this);

    m_iActiveTool = -1;
    createTools();
}

MapDocument::~MapDocument()
{
    qDeleteAll(m_Tools);
    delete m_pScene;
}

MapScene* MapDocument::scene() const
{
    return m_pScene;
}

QColor MapDocument::backgroundColor() const
{
    return m_colBackground;
}

void MapDocument::setBackgroundColor(const QColor &col)
{
    m_colBackground = col;
    m_colBackground.setAlpha(255);
}

const QSet<SceneObject*>& MapDocument::selectedSet() const
{
    return m_SelectedSet;
}

void MapDocument::selectedSetInsert(SceneObject *object)
{
    m_SelectedSet.insert(object);
    BaseTool* t = tool(m_iActiveTool);
    if ( t )
        t->selectedSetChanged();
}

void MapDocument::selectedSetRemove(SceneObject *object)
{
    m_SelectedSet.remove(object);
    BaseTool* t = tool(m_iActiveTool);
    if ( t )
        t->selectedSetChanged();
}

void MapDocument::selectedSetClear()
{
    m_SelectedSet.clear();
    BaseTool* t = tool(m_iActiveTool);
    if ( t )
        t->selectedSetChanged();
}

QColor MapDocument::selectedColor() const
{
    return m_colSelected;
}

void MapDocument::setSelectedColor(const QColor &col)
{
    m_colSelected = col;
}

BaseTool* MapDocument::tool(int index) const
{
    return index >= 0 && index < m_Tools.count() ? m_Tools.at(index) : NULL;
}

BaseTool* MapDocument::tool(const QString &name) const
{
    foreach ( BaseTool* t, m_Tools )
    {
        if ( t->objectName() == name )
            return t;
    }

    return NULL;
}

int MapDocument::toolCount() const
{
    return m_Tools.count();
}

void MapDocument::createTools()
{
    m_Tools.append(new DebugTestTool(this));
    m_Tools.append(new TranslationTool(this));
}

int MapDocument::activeToolIndex() const
{
    return m_iActiveTool;
}

void MapDocument::setActiveToolIndex(int index)
{
    if ( index == m_iActiveTool )
        return;

    BaseTool* oldTool = activeTool();
    m_iActiveTool = index;
    switchTool(oldTool, activeTool());
}

void MapDocument::switchTool(BaseTool *oldTool, BaseTool *newTool)
{
    if ( oldTool )
    {
        oldTool->deactivate();
    }

    if ( newTool )
    {
        newTool->activate();
    }
}

BaseTool* MapDocument::activeTool() const
{
    return tool(m_iActiveTool);
}

InputProcessor* MapDocument::inputProcessor() const
{
    return m_pInputProcessor;
}

UIScene* MapDocument::uiScene() const
{
    return m_pUIScene;
}

// TODO: Make this faster. Everything is computed on demand, sometimes more than once!
QVector3D MapDocument::selectedSetCentroid() const
{
    QVector3D centroid;
    int count = 0;

    for ( QSet<SceneObject*>::const_iterator it = m_SelectedSet.cbegin(); it != m_SelectedSet.cend(); ++it )
    {
        SceneObject* o = *it;
        QVector3D vec = (o->rootToLocal().inverted() * o->computeLocalBounds()).centroid();
        centroid = ((centroid * count) + vec)/(count+1);
        count++;
    }

    return centroid;
}

int MapDocument::toolIndex(const QString &name) const
{
	for ( int i = 0; i < m_Tools.count(); i++ )
	{
		if ( m_Tools.at(i)->objectName() == name )
			return i;
	}

	return -1;
}

bool MapDocument::serialiseToJson(QJsonObject &obj) const
{
    obj.insert(ISerialisable::KEY_IDENTIFIER(), MapDocument::serialiseIdentifier());

    QJsonObject jsonScene;
    m_pScene->serialiseToJson(jsonScene);
    obj.insert("scene", QJsonValue(jsonScene));

    QJsonArray arrBgCol;
    JsonUtil::colorToJsonArray(m_colBackground, arrBgCol);
    obj.insert("backgroundColor", QJsonValue(arrBgCol));

    QJsonArray arrSelectedCol;
    JsonUtil::colorToJsonArray(m_colSelected, arrSelectedCol);
    obj.insert("selectedColor", QJsonValue(arrSelectedCol));

    return true;
}

bool MapDocument::unserialiseFromJson(const QJsonObject &serialisedData)
{
    if ( !validateIdentifier(serialisedData, MapDocument::serialiseIdentifier()) )
        return false;

    QJsonArray arrBgCol = serialisedData.value("backgroundColor").toArray();
    if ( arrBgCol.count() >= 3 )
    {
        setBackgroundColor(JsonUtil::jsonArrayToColor(arrBgCol));
    }

    QJsonArray arrSelectedCol = serialisedData.value("selectedColor").toArray();
    if ( arrSelectedCol.count() >= 3 )
    {
        setSelectedColor(JsonUtil::jsonArrayToColor(arrSelectedCol));
    }

    QJsonValue vScene = serialisedData.value("scene");
    if ( vScene.isObject() )
    {
        return m_pScene->unserialiseFromJson(vScene.toObject());
    }
    else
    {
        return false;
    }
}

QString MapDocument::serialiseIdentifier() const
{
    return staticMetaObject.className();
}

QString MapDocument::filename() const
{
    return m_szFilename;
}

void MapDocument::setFilename(const QString &name)
{
    m_szFilename = name;
}

CalliperMapFile::FileFormat MapDocument::fileFormat() const
{
    return m_iFileFormat;
}

void MapDocument::setFileFormat(CalliperMapFile::FileFormat format)
{
    m_iFileFormat = format;
}
