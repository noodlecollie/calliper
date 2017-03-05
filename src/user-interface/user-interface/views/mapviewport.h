#ifndef MAPVIEWPORT_H
#define MAPVIEWPORT_H

#include "user-interface_global.h"
#include <QOpenGLWidget>

namespace UserInterface
{
    class MapViewport : public QOpenGLWidget
    {
        Q_OBJECT
    public:
        MapViewport(QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
    };
}

#endif // MAPVIEWPORT_H
