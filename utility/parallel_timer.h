#pragma once

#include "timer.h"

namespace DragonLib
{
    class ParallelTimer
    {
    public:
        void Start();
        void Stop(std::string id);
        void AllStop();
        template <class T> uint64_t GetElapsedTime(std::string id);
        template <class T> uint64_t GetElapsedTimeAverage();

    private:
        std::chrono::high_resolution_clock::time_point m_StartTime;
        std::map<std::string, std::chrono::high_resolution_clock::time_point> m_EndTimes;
    };
}
