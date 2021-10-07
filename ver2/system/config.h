#pragma once
// ------------------------------
// Author: Ryouma Kawashima
// Create: 2021/10/07
// Update: 2021/10/07
// ------------------------------
// Description
// 
// ------------------------------

#include "../utility/parameter.h"

namespace DragonLib
{
    // OS
    #define USE_WINDOWS  true

    
    // Graphics API
    #define USE_DIRECTX11   true
    #define USE_DIRECTX12   false


    // Window
    constexpr TCHAR CLASS_NAME[]        = _T("App Class");
    constexpr TCHAR WINDOW_CAPTION[]    = _T("Window Caption");

    constexpr float WINDOW_WIDTH        = 1280.0f;
    constexpr float WINDOW_HEIGHT       = 720.0f;


    // Main Loop
    #define USE_PAUSE true
    #define USE_SLEEP true

    constexpr uint64_t PAUSE_LOOP_NUM = 10;


    // System Timer
    constexpr uint64_t FPS = 60;

    constexpr uint64_t PHYSICS_PER_SECOND   = 20;
    constexpr uint64_t UPDATE_PER_SECOND    = 120;
    constexpr uint64_t DRAW_PER_SECOND      = FPS;
    constexpr uint64_t DEBUG_PER_SECOND     = 1;

    constexpr uint64_t TIMER_FREQUENCY      = 1000000000; // nanoseconds

    constexpr uint64_t PHYSICS_WAIT_TIME    = TIMER_FREQUENCY / PHYSICS_PER_SECOND;
    constexpr uint64_t UPDATE_WAIT_TIME     = TIMER_FREQUENCY / UPDATE_PER_SECOND;
    constexpr uint64_t DRAW_WAIT_TIME       = TIMER_FREQUENCY / DRAW_PER_SECOND;
    constexpr uint64_t DEBUG_WAIT_TIME      = TIMER_FREQUENCY / DEBUG_PER_SECOND;

    constexpr float PHYSICS_DELTA_TIME      = PHYSICS_WAIT_TIME / TIMER_FREQUENCY;
}
