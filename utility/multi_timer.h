#pragma once

#include "timer.h"

namespace DragonLib
{
    class MultiTimer
    {
    public:
        void AddTimer(std::string id);
        void Start(std::string id);
        void StartAll();
        void Stop(std::string id);
        void StopAll();
        template <class T> uint64_t GetElapsedTime(std::string id);
        template <class T> uint64_t GetElapsedTimeAverage();

    private:
        std::map<std::string, Timer> m_Timers;
    };
}
