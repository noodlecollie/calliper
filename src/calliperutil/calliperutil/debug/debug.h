#ifndef DEBUG_H
#define DEBUG_H

#include "calliperutil_global.h"

#ifdef QT_DEBUG
#define CUTL_ASSERT_SUCCESS(_expr) \
    { bool CUTL_success = (_expr); Q_ASSERT(CUTL_success); }
#else
    { (_expr); }
#endif

namespace CalliperUtil
{

}

#endif // DEBUG_H
