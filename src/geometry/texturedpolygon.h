#ifndef TEXTUREDPOLYGON_H
#define TEXTUREDPOLYGON_H

#include <QString>
#include "winding3d.h"
#include <QList>
#include "plane3d.h"

class TexturedPolygon
{
public:
    TexturedPolygon(const Plane3D &plane, const QString &tex);

    Winding3D& winding();
    const Winding3D& winding() const;

    QString texture() const;
    void setTexture(const QString &str);

    QList<int>& vertexIndices();
    const QList<int>& vertexIndices() const;

private:

    Winding3D   m_Winding;
    QString     m_szTexture;
    QList<int>  m_VertexIndices;
};

#endif // TEXTUREDPOLYGON_H
