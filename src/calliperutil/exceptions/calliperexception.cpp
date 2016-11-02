#include "calliperexception.h"

namespace CalliperUtil
{
    void CalliperException::raise() const
    {
        throw *this;
    }

    CalliperException* CalliperException::clone() const
    {
        return new CalliperException(*this);
    }

    CalliperException::CalliperException(const QString &errHint)
        : QException(), m_strErrorHint(errHint)
    {

    }

    QString CalliperException::errorHint() const
    {
        return m_strErrorHint;
    }
}
