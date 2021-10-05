#include <map>
#include "param.h"

#include "multi_timer.h"

using namespace DragonLib;

/*

void MultiTimer::AddTimer(const TCHAR* id)
{
    m_Timers.emplace(id);
}
void MultiTimer::Start(const TCHAR* id)
{
    m_Timers[id].Start();
}
void MultiTimer::StartAll()
{
    for (auto timer : m_Timers)
    {
        timer.second.Start();
    }
}
void MultiTimer::Stop(const TCHAR* id)
{
    m_Timers[id].Stop();
}
void MultiTimer::StopAll()
{
    for (auto timer : m_Timers)
    {
        timer.second.Stop();
    }
}
template <class T>
uint64_t MultiTimer::GetElapsedTime(const TCHAR* id)
{
    return m_Timers[id].GetElapsedTime<T>();
}
template <class T>
uint64_t MultiTimer::GetElapsedTimeAverage()
{
    uint64_t sum = 0;
    uint64_t cnt = 0;

    for (auto timer : m_Timers)
    {
        sum += timer.second.GetElapsedTime<T>();
        cnt++;
    }

    return sum / cnt;
}
*/