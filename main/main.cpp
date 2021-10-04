// -----------------------------------
// Author: Ryouma Kawashima
// Create: 2021/10/04
// Update: 2021/10/04
// -----------------------------------
#include "main.h"

using namespace DragonLib;

#if defined(OS_WINDOWS)

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    UNREFERENCED_PARAMETER(hInstance);
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    UNREFERENCED_PARAMETER(nCmdShow);

    timeBeginPeriod(1);
    Timer systemTimer;

    

    timeEndPeriod(1);
    return 0;
}

#else

int main(void)
{
    return 0;
}

#endif



