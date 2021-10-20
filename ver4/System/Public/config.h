#pragma once

#include "../../Utility/Public/parameter.h"
#include "../../Utility/Public/timer.h"

namespace DragonLib
{
    // Renderer
    #define DIRECTX_12 0x01
    #define DIRECTX_11 0x02

    #define RENDERER_TYPE DIRECTX_12

    // Window
    constexpr TCHAR CLASS_NAME[]        = _T("App Class");
    constexpr TCHAR WINDOW_CAPTION[]    = _T("Window Caption");

    constexpr u32 WINDOW_WIDTH  = 1280;
    constexpr u32 WINDOW_HEIGHT = 720;

    // Timer
    #define MILLI 1000
    #define MICRO 1000000
    #define NANO  1000000000
    constexpr u64 FPS = 60;
    constexpr u64 TIMER_FREQUENCY = NANO;
}


