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
    explicit BaseTool(const QString &name, MapDocument* document);
    virtual ~BaseTool();    // Subclasses MUST handle deactivation here.
    virtual QString toolName() const;
    MapDocument* document() const;

    // Activation
    void activate();
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

public slots:
    void selectedSetChanged();

protected:
    virtual void vActivate() {}
    virtual void vDeactivate() {}
    virtual void vMousePress(QMouseEvent*) {}
    virtual void vMouseMove(QMouseEvent*) {}
    virtual void vMouseRelease(QMouseEvent*) {}
    virtual void vWheel(QWheelEvent*) {}
    virtual void vKeyPress(QKeyEvent*) {}
    virtual void vKeyRelease(QKeyEvent*) {}
    virtual void vSelectedSetChanged() {}

    MapDocument*    m_pDocument;
    bool            m_bActive;
};

#endif // BASETOOL_H
