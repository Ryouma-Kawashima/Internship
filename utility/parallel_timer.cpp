#include <map>
#include "param.h"

#include "parallel_timer.h"

using namespace DragonLib;

void ParallelTimer::Start()
{
    m_StartTime = std::chrono::high_resolution_clock::now();
}
void ParallelTimer::Stop(const TCHAR* id)
{
    m_EndTimes[id] = std::chrono::high_resolution_clock::now();
}
void ParallelTimer::AllStop()
{
    auto endTime = std::chrono::high_resolution_clock::now();

    for (auto time : m_EndTimes)
    {
        time.second = endTime;
    }
}
template <class T>
uint64_t ParallelTimer::GetElapsedTime(const TCHAR* id)
{
    return m_EndTimes[id] - m_StartTime;
}
template <class T> 
uint64_t ParallelTimer::GetElapsedTimeAverage()
{
    uint64_t sum = 0;
    uint64_t cnt = 0;

    for (auto time : m_EndTimes)
    {
        sum = time.second - m_StartTime;
        cnt++;
    }

    return sum / cnt;
}
