#ifndef CALLIPERPROJECT_H
#define CALLIPERPROJECT_H

#include "model_global.h"
#include "core/datachangenotifier.h"
#include "calliperprojectmetadata.h"

namespace Model
{
    class MODELSHARED_EXPORT CalliperProject : public DataChangeNotifier
    {
        Q_OBJECT
    public:
        explicit CalliperProject(QObject* parent = 0);

        void clear();

        CalliperProjectMetadata* metadata();
        const CalliperProjectMetadata* metadata() const;

    private:
        CalliperProjectMetadata* m_pMetadata;
    };
}

#endif // CALLIPERPROJECT_H
