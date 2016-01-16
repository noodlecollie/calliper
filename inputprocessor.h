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
};

#endif // INPUTPROCESSOR_H
