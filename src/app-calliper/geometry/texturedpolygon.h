#ifndef TEXTUREDPOLYGON_H
#define TEXTUREDPOLYGON_H

#include <QString>
#include "winding3d.h"
#include <QList>
#include "plane3d.h"

class TexturedPolygon : public Winding3D
{
public:
    TexturedPolygon(const Plane3D &plane, const QString &tex);

    QString texture() const;
    void setTexture(const QString &str);

private:
    QString     m_szTexture;
};

#endif // TEXTUREDPOLYGON_H
