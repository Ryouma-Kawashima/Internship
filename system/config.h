#pragma once

namespace DragonLib
{
    // Common
    #define SAFE_RELEASE(p) {if(p != nullptr) (p)->Release(); p = nullptr;}

    // Main
    #define OS_WINDOWS  true
    #define OS_LINUX    false
    #define OS_MAC      false

    #define USE_PAUSE   true
    #define USE_SLEEP   true


    // Render
    #define GRAPHIC_DIRECTX11   true
    #define GRAPHIC_DIRECTX12   false
    #define GRAPHIC_OPENGL      false
    #define GRAPHIC_VULKAN      false

    enum RenderSetting
    {
        // ç¿ïWån
        CoordSystem_Left,
        CoordSystem_Right,
    };

    constexpr uint32_t COORD_SYSTEM = CoordSystem_Left;

    constexpr float BG_COLOR_R = 0.0f; // BackGround Color R
    constexpr float BG_COLOR_G = 0.5f; // BackGround Color G
    constexpr float BG_COLOR_B = 0.5f; // BackGround Color B


    // System Timer
    constexpr uint64_t TIMER_FREQUENCY  = 1000000000; // nanoseconds ( 10 ^ -9 )
    constexpr uint64_t DEBUG_FREQUENCY  = 1000000000; // nanoseconds ( 10 ^ -9 )
    constexpr uint64_t FPS = 60;

    constexpr uint64_t EXEC_WAIT_TIME   = TIMER_FREQUENCY / FPS;
    constexpr uint64_t DEBUG_WAIT_TIME  = DEBUG_FREQUENCY;
    constexpr uint64_t DEBUG_PER_SECOND = TIMER_FREQUENCY / DEBUG_FREQUENCY;

    constexpr uint32_t PAUSE_LOOP_NUM = 5;


    // Window
    constexpr TCHAR CLASS_NAME[]    = _T("App Class");
    constexpr TCHAR WINDOW_NAME[]   = _T("Window Name");

    constexpr uint32_t WINDOW_WIDTH     = 1280;
    constexpr uint32_t WINDOW_HEIGHT    = 720;


    // Math
    #define USE_SIMD true
}
