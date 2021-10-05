
#include <map>
#include <string>

#include "config.h"
#include "window.h"
#include "../utility/parallel_timer.h"

using namespace DragonLib;

#if defined(OS_WINDOWS)

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hInstance);
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	Window window( WINDOW_WIDTH, WINDOW_HEIGHT );
	window.SetWindowShow(nCmdShow);

	timeBeginPeriod(1);

	ParallelTimer timer;
	timer.Start();

	MSG msg;
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			timer.Stop(std::string("ExecutionTimer"));

			/*
			if ((dwCurrentTime - dwExecLastTime) >= (TIMER_FREQUENCY / FPS))
			{
				dwExecLastTime = dwCurrentTime;
				dwTickPerSec++;

				// FPS‚ÌŒv‘ª
				#if defined(__DEBUG) || defined(_DEBUG)

				if (dwExecLastTime > dwLastElapsedTime + DEBUG_FREQUENCY)
				{
					//DebugPrintf("FPS: %ld\n", dwTickPerSec * TIMER_FREQUENCY / DEBUG_FREQUENCY);

					dwLastElapsedTime = dwExecLastTime;
					dwTickPerSec = 0;
				}

				#endif
			}
			else
			{
			#ifdef USE_PAUSE
				for (uint32_t i = 0; i < PAUSE_NUM; i++)
				{
					_mm_pause();
				}
			#endif
			#ifdef USE_SLEEP
				Sleep(0);
			#endif
			}
			*/
		}
	}

	timeEndPeriod(1);

	return (int)msg.wParam;
}

#elif defined(OS_LINUX)

int main(int argc, char* argv[])
{
	return 0;
}

#else

int main(void)
{
	return 0;
}
	
#endif


