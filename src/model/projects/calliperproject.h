#ifndef CALLIPERPROJECT_H
#define CALLIPERPROJECT_H

#include "model_global.h"
#include "calliperprojectmetadata.h"

namespace Model
{
    class CalliperProject
    {
    public:
        CalliperProject();

        void clear();

        CalliperProjectMetadata& metadata();
        const CalliperProjectMetadata& metadata() const;

    private:
        CalliperProjectMetadata m_Metadata;
    };
}

#endif // CALLIPERPROJECT_H
