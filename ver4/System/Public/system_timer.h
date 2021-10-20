#pragma once

#include "config.h"
#include "../../Utility/Public/timer.h"

namespace DragonLib
{
    constexpr NanoSeconds WAIT_TIME = NanoSeconds(TIMER_FREQUENCY / FPS);

    class SystemTimer : public Timer
    {
    public:
        SystemTimer(u32 fps)
        {
            SetWaitTimeBasedOnFPS(fps);
        }

        bool IsOverTime()
        {
            return m_StartTime + m_WaitTime < m_EndTime;
        }
        void SetWaitTimeBasedOnFPS(u32 fps)
        {
            m_WaitTime = NanoSeconds(TIMER_FREQUENCY / fps);
        }

    private:
        NanoSeconds m_WaitTime;
    };
}
