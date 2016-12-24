#include "calliperproject.h"

namespace Model
{
    CalliperProject::CalliperProject()
    {

    }

    void CalliperProject::clear()
    {
        m_Metadata.clear();
    }

    CalliperProjectMetadata& CalliperProject::metadata()
    {
        return m_Metadata;
    }

    const CalliperProjectMetadata& CalliperProject::metadata() const
    {
        return m_Metadata;
    }
}
