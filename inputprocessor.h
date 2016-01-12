#ifndef INPUTPROCESSOR_H
#define INPUTPROCESSOR_H

#include <QObject>

class MapDocument;
class QKeyEvent;
class QMouseEvent;
class QWheelEvent;
class Viewport;

class InputProcessor : public QObject
{
    Q_OBJECT
public:
    explicit InputProcessor(MapDocument *document);

    MapDocument* document() const;

    virtual bool eventFilter(QObject *watched, QEvent *event);
signals:

public slots:

private:
    bool filterKeyPress(QObject* watched, QKeyEvent* e);
    bool filterKeyRelease(QObject* watched, QKeyEvent* e);
    bool filterMousePress(QObject* watched, QMouseEvent* e);
    bool filterMouseMove(QObject* watched, QMouseEvent* e);
    bool filterMouseRelease(QObject* watched, QMouseEvent* e);
    bool filterWheel(QObject* watched, QWheelEvent* e);

    void setSelectionToObjectAtPixel(Viewport* viewport, const QPoint &pos);
};

#endif // INPUTPROCESSOR_H
