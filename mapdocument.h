#ifndef MAPDOCUMENT_H
#define MAPDOCUMENT_H

#include <QObject>
#include <QColor>
#include <QSet>

class Scene;
class SceneObject;

class MapDocument : public QObject
{
    Q_OBJECT
public:
    explicit MapDocument(QObject* parent = 0);
    ~MapDocument();

    Scene* scene() const;

    QColor backgroundColor() const;
    void setBackgroundColor(const QColor &col);

    const QSet<SceneObject*>& selectedSet() const;
    void selectedSetInsert(SceneObject* object);
    void selectedSetRemove(SceneObject* object);
    void selectedSetClear();

    QColor selectedColor() const;
    void setSelectedColor(const QColor &col);

private:
    Scene*  m_pScene;
    QColor  m_colBackground;
    QSet<SceneObject*>  m_SelectedSet;
    QColor  m_colSelected;
};

#endif // MAPDOCUMENT_H
