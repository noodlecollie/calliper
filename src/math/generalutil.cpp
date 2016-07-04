#include "generalutil.h"

#include <QVector3D>
#include <QStringList>

namespace GeneralUtil
{
    // TODO: Error handling.
    QVector3D vectorFromVmfCoord(const QString &coord)
    {
        QStringList list = coord.split(" ");
        list[0] = list[0].right(list[0].length() - 1);
        list[2] = list[2].left(list[2].length() - 1);

        return QVector3D(list.at(0).toFloat(),
                         list.at(1).toFloat(),
                         list.at(2).toFloat());
    }

    // TODO: Error handling.
    void vectorsFromVmfCoords(const QString &coords, QVector3D &v0, QVector3D &v1, QVector3D &v2)
    {
        int sep1 = coords.indexOf(')') + 1;
        int sep2 = coords.indexOf(')', sep1) + 1;

        QString coord0 = coords.mid(0, sep1);
        QString coord1 = coords.mid(sep1+1, sep2-sep1-1);
        QString coord2 = coords.mid(sep2+1);

        v0 = vectorFromVmfCoord(coord0);
        v1 = vectorFromVmfCoord(coord1);
        v2 = vectorFromVmfCoord(coord2);
    }
}

