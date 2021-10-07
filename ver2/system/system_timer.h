#pragma once

namespace DragonLib
{
    enum class TimerType : int
    {
        Update,
        Draw,
        Debug,
        Physics,
    };

    class SystemTimer
    {
    public:
        void ResetUpdateTimer();
        void ResetDrawTimer();
        void ResetDebugTimer();
        void ResetPhysicsTimer();

        bool CheckUpdateTimer();
        bool CheckDrawTimer();
        bool CheckDebugTimer();
        bool CheckPhysicsTimer();

        float GetUpdateDeltaTime();
        float GetDrawDeltaTime();
        float GetDebugDeltaTime();
        float GetPhysicsDeltaTime();

        uint32_t GetTickCount(TimerType type);

    private:
        Timer m_UpdateTimer;
        Timer m_DrawTimer;
        Timer m_DebugTimer;
        std::chrono::high_resolution_clock::time_point m_PhysicsLastTime;

        std::array<uint32_t, 4U> m_TickCount;
        std::array<double,   4U> m_ProgressTime;
    };
}
