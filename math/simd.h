#pragma once

#if OS_WINDOWS

#include <intrin.h>

#elif OS_LINUX

#include <x86intrin.h>

#endif

#define CAN_BE_USED_SIMD USE_SIMD && defined(__IMM_INCLUDE)

