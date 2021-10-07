#pragma once

#include <chrono>
#include <variant>

namespace DragonLib
{
    // Durations
    typedef std::chrono::nanoseconds    NanoSeconds;
    typedef std::chrono::microseconds   MicroSeconds;
    typedef std::chrono::milliseconds   MilliSeconds;
    typedef std::chrono::seconds        Seconds;
    typedef std::chrono::minutes        Minutes;
    typedef std::chrono::hours          Hours;

    class Timer
    {
    public:
        void Start()
        {
            m_StartTime = std::chrono::high_resolution_clock::now();
        }
        void Stop()
        {
            m_EndTime = std::chrono::high_resolution_clock::now();
        }
        template <typename T>
        uint64_t GetStartTime()
        {
            return static_cast<uint64_t>(std::chrono::duration_cast<T>(m_StartTime.time_since_epoch()).count());
        }
        template <typename T>
        uint64_t GetEndTime()
        {
            return static_cast<uint64_t>(std::chrono::duration_cast<T>(m_EndTime.time_since_epoch()).count());
        }
        template <typename T>
        uint64_t GetElapsedTime()
        {
            return static_cast<uint64_t>(std::chrono::duration_cast<T>(m_EndTime - m_StartTime).count());
        }

    private:
        std::chrono::high_resolution_clock::time_point m_StartTime;
        std::chrono::high_resolution_clock::time_point m_EndTime;
    };
}
