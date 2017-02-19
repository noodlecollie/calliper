#ifndef VMFDATAMODEL_H
#define VMFDATAMODEL_H

#include "app-calliper_global.h"
#include <QObject>
#include "model/scene/mapscene.h"

namespace AppCalliper
{
    class VmfDataModel : public QObject
    {
        Q_OBJECT
    public:
        VmfDataModel();

    private:
        Model::MapScene* m_pScene;
    };
}

#endif // VMFDATAMODEL_H
