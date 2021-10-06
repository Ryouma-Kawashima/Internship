#pragma once

#ifdef GetCurrentTime
#undef GetCurrentTime
#endif

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
        uint64_t GetStartTime()
        {
            return std::chrono::duration_cast<T>(m_StartTime.time_since_epoch()).count();
        }
        template<class T>
        uint64_t GetEndTime()
        {
            return std::chrono::duration_cast<T>(m_EndTime.time_since_epoch()).count();
        }
        template <class T>
        uint64_t GetElapsedTime()
        {
            return std::chrono::duration_cast<T>(m_EndTime - m_StartTime).count();
        }
        template <class T>
        static uint64_t GetCurrentTime()
        {
            return std::chrono::duration_cast<T>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
        }

    private:
        std::chrono::high_resolution_clock::time_point m_StartTime;
        std::chrono::high_resolution_clock::time_point m_EndTime;
    };
}
