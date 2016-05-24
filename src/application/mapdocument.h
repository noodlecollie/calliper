#ifndef MAPDOCUMENT_H
#define MAPDOCUMENT_H

#include <QObject>
#include <QColor>
#include <QSet>
#include "boundingbox.h"
#include "iserialisable.h"
#include "callipermapfile.h"

class MapScene;
class UIScene;
class SceneObject;
class BaseTool;
class InputProcessor;

class MapDocument : public QObject, public ISerialisable
{
    Q_OBJECT
public:
    explicit MapDocument(QObject* parent = 0);
    ~MapDocument();

    MapScene* scene() const;
    UIScene* uiScene() const;

    QString filename() const;
    void setFilename(const QString &name);

    CalliperMapFile::FileFormat fileFormat() const;
    void setFileFormat(CalliperMapFile::FileFormat format);

    QColor backgroundColor() const;
    void setBackgroundColor(const QColor &col);

    const QSet<SceneObject*>& selectedSet() const;
    void selectedSetInsert(SceneObject* object);
    void selectedSetRemove(SceneObject* object);
    void selectedSetClear();
    QVector3D selectedSetCentroid() const;

    QColor selectedColor() const;
    void setSelectedColor(const QColor &col);

    BaseTool* tool(int index) const;
    BaseTool* tool(const QString &name) const;
    int toolCount() const;
	int toolIndex(const QString &name) const;

    int activeToolIndex() const;
    void setActiveToolIndex(int index);
    BaseTool* activeTool() const;

    InputProcessor* inputProcessor() const;

    virtual bool serialiseToJson(QJsonObject &obj) const;
    virtual QString serialiseIdentifier() const;
    bool unserialiseFromJson(const QJsonObject &serialisedData);

private:
    void createTools();
    void switchTool(BaseTool* oldTool, BaseTool* newTool);

    MapScene*  m_pScene;
    UIScene*  m_pUIScene;
    QColor  m_colBackground;
    QSet<SceneObject*>  m_SelectedSet;
    QColor  m_colSelected;
    QList<BaseTool*>    m_Tools;
    int m_iActiveTool;
    QString m_szFilename;
    CalliperMapFile::FileFormat m_iFileFormat;

    InputProcessor* m_pInputProcessor;
};

#endif // MAPDOCUMENT_H
