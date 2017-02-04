#include "calliperproject.h"

namespace Model
{
    CalliperProject::CalliperProject(QObject* parent)
        : DataChangeNotifier(parent),
          m_pMetadata(new CalliperProjectMetadata(this))
    {
        connectDataChangedSignals(m_pMetadata);
    }

    void CalliperProject::clear()
    {
        m_pMetadata->clear();
    }

    CalliperProjectMetadata* CalliperProject::metadata()
    {
        return m_pMetadata;
    }

    const CalliperProjectMetadata* CalliperProject::metadata() const
    {
        return m_pMetadata;
    }
}
