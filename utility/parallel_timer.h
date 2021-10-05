#pragma once

#include "timer.h"

namespace DragonLib
{
    class ParallelTimer
    {
    public:
        void AddId(const TCHAR* id);
        void Start();
        void Stop(const TCHAR* id);
        void AllStop();
        template <class T> uint64_t GetElapsedTime(const TCHAR* id);
        template <class T> uint64_t GetElapsedTimeAverage();

    private:
        std::chrono::high_resolution_clock::time_point m_StartTime;
        std::map<const TCHAR*, std::chrono::high_resolution_clock::time_point> m_EndTimes;
    };
}
