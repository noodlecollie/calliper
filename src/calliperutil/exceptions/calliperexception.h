#ifndef CALLIPEREXCEPTION_H
#define CALLIPEREXCEPTION_H

#include "calliperutil_global.h"
#include <QException>
#include <QString>

namespace NS_CALLIPERUTIL
{
    class CALLIPERUTILSHARED_EXPORT CalliperException : public QException
    {
    public:
        virtual void raise() const override;
        virtual CalliperException* clone() const override;

        QString errorHint() const;

    protected:
        QString m_strErrorHint;
    };
}

#endif // CALLIPEREXCEPTION_H
