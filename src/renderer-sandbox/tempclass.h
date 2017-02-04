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
    void handleSelect();
    void handleMaximise();
    void handleClose();
};

#endif // TEMPCLASS_H
