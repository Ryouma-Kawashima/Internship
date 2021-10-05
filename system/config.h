#pragma once

#include "../utility/param.h"

namespace DragonLib
{
    // Main
    #define OS_WINDOWS
    //#define OS_LINUX
    //#define OS_MAC

    #define USE_PAUSE
    #define USE_SLEEP

    // Render
    #define ENGINE_DIRECTX11

    //#define ENGINE_DIRECTX12
    //#define ENGINE_OPENGL
    //#define ENGINE_VULKAN

    // System Timer
    constexpr uint64_t TIMER_FREQUENCY = 1000;
    constexpr uint64_t DEBUG_FREQUENCY = 1000;
    constexpr uint64_t FPS = 60;

    constexpr uint32_t PAUSE_NUM = 5;

    // Window
    constexpr TCHAR CLASS_NAME[]    = _T("App Class");
    constexpr TCHAR WINDOW_NAME[]   = _T("Window Name");

    constexpr uint32_t WINDOW_WIDTH     = 1280;
    constexpr uint32_t WINDOW_HEIGHT    = 720;

    
}
