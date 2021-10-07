#include <array>

#include "../utility/parameter.h"
#include "../system/config.h"

#include "../utility/timer.h"

#include "system_timer.h"

using namespace DragonLib;

void SystemTimer::ResetUpdateTimer()
{
    m_UpdateTimer.Start();
    m_TickCount[static_cast<int>(TimerType::Update)] = 0;
}
void SystemTimer::ResetDrawTimer()
{
    m_DrawTimer.Start();
    m_TickCount[static_cast<int>(TimerType::Draw)] = 0;
}
void SystemTimer::ResetDebugTimer()
{
    m_DebugTimer.Start();
    m_TickCount[static_cast<int>(TimerType::Debug)] = 0;
}
void SystemTimer::ResetPhysicsTimer()
{
    m_PhysicsLastTime = std::chrono::high_resolution_clock::now();
    m_TickCount[static_cast<int>(TimerType::Physics)] = 0;
}

bool SystemTimer::CheckUpdateTimer()
{
    m_UpdateTimer.Stop();
    if (UPDATE_WAIT_TIME < m_UpdateTimer.GetElapsedTime<NanoSeconds>())
    {
        m_TickCount[static_cast<int>(TimerType::Update)]++;
        return true;
    }
    else
    {
        return false;
    }
}
bool SystemTimer::CheckDrawTimer()
{
    m_DrawTimer.Stop();
    if (DRAW_WAIT_TIME < m_DrawTimer.GetElapsedTime<NanoSeconds>())
    {
        m_TickCount[static_cast<int>(TimerType::Draw)]++;
        return true;
    }
    else
    {
        return false;
    }
}
bool SystemTimer::CheckDebugTimer()
{
    m_DebugTimer.Stop();
    if (DEBUG_WAIT_TIME < m_DebugTimer.GetElapsedTime<NanoSeconds>())
    {
        m_TickCount[static_cast<int>(TimerType::Debug)]++;
        return true;
    }
    else
    {
        return false;
    }
}
bool SystemTimer::CheckPhysicsTimer()
{
    if (PHYSICS_WAIT_TIME < static_cast<uint64_t>((std::chrono::high_resolution_clock::now() - m_PhysicsLastTime).count()))
    {
        m_PhysicsLastTime += NanoSeconds(PHYSICS_WAIT_TIME);
        m_TickCount[static_cast<int>(TimerType::Physics)]++;
        return true;
    }
    else
    {
        return false;
    }
}

float SystemTimer::GetUpdateDeltaTime()
{
    return static_cast<float>(m_UpdateTimer.GetElapsedTime<NanoSeconds>());
}
float SystemTimer::GetDrawDeltaTime()
{
    return static_cast<float>(m_UpdateTimer.GetElapsedTime<NanoSeconds>());
}
float SystemTimer::GetDebugDeltaTime()
{
    return static_cast<float>(m_UpdateTimer.GetElapsedTime<NanoSeconds>());
}
float SystemTimer::GetPhysicsDeltaTime()
{
    return PHYSICS_DELTA_TIME;
}

uint32_t SystemTimer::GetTickCount(TimerType type)
{
    return m_TickCount[static_cast<int>(type)];
}
