#ifndef BASETOOL_H
#define BASETOOL_H

#include <QObject>
#include <QString>
#include <QSet>

class MapDocument;
class QMouseEvent;
class QKeyEvent;
class QWheelEvent;
class SceneObject;

class BaseTool : public QObject
{
    Q_OBJECT
public:
    explicit BaseTool(const QString &name);
    virtual ~BaseTool();
    virtual QString toolName() const;

    // Activation
    void activate(MapDocument* doc);
    void deactivate();
    bool isActive() const;

    // Mouse input
    void mousePressEvent(QMouseEvent* e);
    void mouseMoveEvent(QMouseEvent* e);
    void mouseReleaseEvent(QMouseEvent* e);
    void wheelEvent(QWheelEvent* e);

    // Keyboard input
    void keyPressEvent(QKeyEvent* e);
    void keyReleaseEvent(QKeyEvent* e);

    // SceneObject management
    void addSceneObject(SceneObject* obj);
    void removeSceneObject(SceneObject* obj);
    void clearSceneObjects();
    bool containsSceneObject(SceneObject* obj) const;

protected:
    virtual void vActivate() {}
    virtual void vDeactivate() {}
    virtual void vMousePress(QMouseEvent*) {}
    virtual void vMouseMove(QMouseEvent*) {}
    virtual void vMouseRelease(QMouseEvent*) {}
    virtual void vWheel(QWheelEvent*) {}
    virtual void vKeyPress(QKeyEvent*) {}
    virtual void vKeyRelease(QKeyEvent*) {}
    virtual void vSceneObjectAdded(SceneObject*) {}
    virtual void vSceneObjectRemoved(SceneObject*) {}
    virtual void vSceneObjectsCleared() {}

    MapDocument*    m_pDocument;
    bool            m_bActive;
    QSet<SceneObject*>  m_SceneObjects;
};

#endif // BASETOOL_H
