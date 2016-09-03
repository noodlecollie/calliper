#ifndef RAYTRACECONTACT_H
#define RAYTRACECONTACT_H

#include "model_global.h"
#include <QVector3D>
#include <QRgb>

namespace NS_MODEL
{
    class RayTraceContact
    {
    public:
        RayTraceContact() :
            rayParameter((float)qInf()), color(0xff000000), normal()
        {
        }

        RayTraceContact(float parameter, QRgb col = 0xff000000, const QVector3D &nrm = QVector3D()) :
            rayParameter(parameter), color(col), normal(nrm)
        {
        }

        float rayParameter;
        QRgb color;
        QVector3D normal;
    };
}

#endif // RAYTRACECONTACT_H
