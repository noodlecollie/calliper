#ifndef VMFDATAMODEL_H
#define VMFDATAMODEL_H

#include "app-calliper_global.h"
#include <QObject>
#include "model/scene/basicscene.h"

namespace AppCalliper
{
    class VmfDataModel : public QObject
    {
        Q_OBJECT
    public:
        VmfDataModel();

    private:
        Model::BasicScene* m_pScene;
    };
}

#endif // VMFDATAMODEL_H
