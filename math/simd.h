#pragma once

#if OS_WINDOWS

#include <intrin.h>

#else

#include <x86intrin.h>

#endif

#define CAN_BE_USED_SIMD USE_SIMD

