#include "mapdocument.h"
#include "scene.h"
#include "scenefactory.h"
#include "sceneobject.h"

MapDocument::MapDocument(QObject *parent) : QObject(parent)
{
    m_pScene = SceneFactory::defaultScene(this);
    m_colBackground = QColor::fromRgb(0xff262626);
    m_colSelected = QColor::fromRgb(0xffff0000);
}

MapDocument::~MapDocument()
{
    delete m_pScene;
}

Scene* MapDocument::scene() const
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
}

void MapDocument::selectedSetRemove(SceneObject *object)
{
    m_SelectedSet.remove(object);
}

void MapDocument::selectedSetClear()
{
    m_SelectedSet.clear();
}

QColor MapDocument::selectedColor() const
{
    return m_colSelected;
}

void MapDocument::setSelectedColor(const QColor &col)
{
    m_colSelected = col;
}
