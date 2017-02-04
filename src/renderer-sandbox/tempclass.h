#ifndef TEMPCLASS_H
#define TEMPCLASS_H

#include <QObject>

class TempClass : public QObject
{
    Q_OBJECT
public:
    explicit TempClass(QObject *parent = 0);

signals:

public slots:
    void handleSelect(int itemId);
    void handleMaximise(int itemId);
    void handleClose(int itemId);
};

#endif // TEMPCLASS_H
