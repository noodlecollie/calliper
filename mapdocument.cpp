#include "mapdocument.h"
#include "scene.h"
#include "scenefactory.h"

MapDocument::MapDocument(QObject *parent) : QObject(parent)
{
    m_pScene = SceneFactory::defaultScene(this);
    m_colBackground = QColor::fromRgb(0xFF262626);
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
