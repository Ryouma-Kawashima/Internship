#pragma once

#include "parameter.h"

#include <chrono>

namespace DragonLib
{
    using NanoSeconds   = std::chrono::nanoseconds;
    using MicroSeconds  = std::chrono::microseconds;
    using MilliSeconds  = std::chrono::milliseconds;
    using Seconds       = std::chrono::seconds;
    using Minutes       = std::chrono::minutes;
    using Hours         = std::chrono::hours;

    class Timer
    {
    public:
        Timer() noexcept = default;
        ~Timer() noexcept = default;

        void Start()
        {
            m_StartTime = std::chrono::high_resolution_clock::now();
        }
        void End()
        {
            m_EndTime = std::chrono::high_resolution_clock::now();
        }
        template <typename T>
        uint64_t GetElapsedTime()
        {
            return static_cast<uint64_t>(std::chrono::duration_cast<T>(m_EndTime - m_StartTime).count());
        }

    protected:
        std::chrono::high_resolution_clock::time_point m_StartTime;
        std::chrono::high_resolution_clock::time_point m_EndTime;
    };
}

