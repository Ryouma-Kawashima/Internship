#pragma once

#include <chrono>

namespace DragonLib
{
    class Timer
    {
    public:
        typedef std::chrono::nanoseconds    NanoSeconds;
        typedef std::chrono::microseconds   MicroSeconds;
        typedef std::chrono::milliseconds   MilliSeconds;
        typedef std::chrono::seconds        Seconds;
        typedef std::chrono::minutes        Minutes;
        typedef std::chrono::hours          Hours;

    public:
        void Start()
        {
            m_StartTime = std::chrono::high_resolution_clock::now();
        }
        void Stop()
        {
            m_EndTime = std::chrono::high_resolution_clock::now();
        }
        template <class T>
        uint64_t GetElapsedTime()
        {
            return std::chrono::duration_cast<T>(m_EndTime - m_StartTime);
        }

    private:
        std::chrono::high_resolution_clock::time_point m_StartTime;
        std::chrono::high_resolution_clock::time_point m_EndTime;
    };
}
