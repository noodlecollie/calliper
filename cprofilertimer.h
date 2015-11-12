#ifndef CPROFILERTIMER_H
#define CPROFILERTIMER_H

#include <QString>
#include <QTime>

// Create an instance of this class on the stack when you wish to begin profiling.
// Just before the instance falls out of scope, the function name and amount of
// time the instance was alive for will be printed to debug.
class CProfilerTimer
{
public:
    CProfilerTimer(const QString &function);
    ~CProfilerTimer();
    
private:
    QString m_FunctionName;
    QTime   m_Time;
};

#endif // CPROFILERTIMER_H
