#ifndef MAPDOCUMENT_H
#define MAPDOCUMENT_H

#include <QObject>
#include <QColor>
#include <QSet>

class MapScene;
class SceneObject;
class BaseTool;
class InputProcessor;

class MapDocument : public QObject
{
    Q_OBJECT
public:
    explicit MapDocument(QObject* parent = 0);
    ~MapDocument();

    MapScene* scene() const;

    QColor backgroundColor() const;
    void setBackgroundColor(const QColor &col);

    const QSet<SceneObject*>& selectedSet() const;
    void selectedSetInsert(SceneObject* object);
    void selectedSetRemove(SceneObject* object);
    void selectedSetClear();

    QColor selectedColor() const;
    void setSelectedColor(const QColor &col);

    BaseTool* tool(int index) const;
    BaseTool* tool(const QString &name) const;
    int toolCount() const;

    int activeToolIndex() const;
    void setActiveToolIndex(int index);
    BaseTool* activeTool() const;

    InputProcessor* inputProcessor() const;

private:
    void createTools();
    void switchTool(BaseTool* oldTool, BaseTool* newTool);

    MapScene*  m_pScene;
    QColor  m_colBackground;
    QSet<SceneObject*>  m_SelectedSet;
    QColor  m_colSelected;
    QList<BaseTool*>    m_Tools;
    int m_iActiveTool;

    InputProcessor* m_pInputProcessor;
};

#endif // MAPDOCUMENT_H
