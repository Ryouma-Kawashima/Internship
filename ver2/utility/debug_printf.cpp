#include "debug_printf.h"

#include <cstdio>
#include <malloc.h>
#include <windows.h>

int DragonLib::DebugPrintf(const char* format, ...)
{
#if defined(__DEBUG) || defined(_DEBUG)

    size_t  len = 0;
    char* buf = nullptr;
    va_list ap = nullptr;

    va_start(ap, format);
    len = _vscprintf(format, ap) + 1;
    buf = static_cast<char*>_malloca(len * sizeof(char));
    len = vsnprintf(buf, len, format, ap);
    va_end(ap);

    OutputDebugStringA(buf);
    _freea(buf);

    return static_cast<int>(len);

#else

    UNREFEREENCED_PARAMETER(format);
    return 0;

#endif
}
