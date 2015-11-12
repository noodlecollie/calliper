#include "cprofilertimer.h"
#include <QtDebug>

CProfilerTimer::CProfilerTimer(const QString &function)
{
    m_FunctionName = function;
    m_Time.start();
}

CProfilerTimer::~CProfilerTimer()
{
    float perSec = 0;
    int t = m_Time.elapsed();
    if ( t != 0 )
    {
        perSec = 1000.0f/(float)t;
    }
    
    qDebug().nospace() << "Time elapsed from function " << m_FunctionName << ": " << m_Time.elapsed() << "ms (" << perSec << "/sec)";
}
