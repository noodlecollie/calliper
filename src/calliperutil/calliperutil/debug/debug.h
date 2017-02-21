#ifndef DEBUG_H
#define DEBUG_H

#include "calliperutil_global.h"

#ifdef QT_DEBUG
#define CUTL_ASSERT_SUCCESS(_expr) \
    { bool _CUTL_success = (_expr); Q_ASSERT_X(_CUTL_success, Q_FUNC_INFO, "Task did not execute successfully."); }
#else
#define CUTL_ASSERT_SUCCESS(_expr) _expr;
#endif

namespace CalliperUtil
{

}

#endif // DEBUG_H
