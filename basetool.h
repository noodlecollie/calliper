#ifndef BASETOOL_H
#define BASETOOL_H

#include <QObject>
#include <QString>

class MapDocument;
class QMouseEvent;
class QKeyEvent;
class QWheelEvent;

class BaseTool : public QObject
{
    Q_OBJECT
public:
    explicit BaseTool(const QString &name);
    virtual ~BaseTool();

    // Activation
    virtual void activate();
    virtual void deactivate();

    // Mouse input
    virtual void mousePressEvent(QMouseEvent*) {}
    virtual void mouseMoveEvent(QMouseEvent*) {}
    virtual void mouseReleaseEvent(QMouseEvent*) {}
    virtual void wheelEvent(QWheelEvent*) {}

    // Keyboard input
    virtual void keyPressEvent(QKeyEvent*) {}
    virtual void keyReleaseEvent(QKeyEvent*) {}

protected:

};

#endif // BASETOOL_H
