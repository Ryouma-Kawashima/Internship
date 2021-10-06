#include "main.h"

using namespace DragonLib;

#if defined(OS_WINDOWS)

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, TCHAR* lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hInstance);
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);


	Window window( WINDOW_WIDTH, WINDOW_HEIGHT );
	window.SetWindowShow(nCmdShow);

	RenderAPI renderer;

	timeBeginPeriod(1);
	
	uint32_t tickCount = 0;
	Timer execTimer, debugTimer;
	execTimer.Start();
	debugTimer.Start();


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
			execTimer.Stop();

			if (execTimer.GetElapsedTime<Timer::NanoSeconds>() > EXEC_WAIT_TIME)
			{
				execTimer.Start();
				tickCount++;

				// FPS‚ÌŒv‘ª
				#if defined( __DEBUG ) || defined( _DEBUG )

				if (execTimer.GetStartTime<Timer::NanoSeconds>() > 
					debugTimer.GetStartTime<Timer::NanoSeconds>() + DEBUG_WAIT_TIME)
				{
					DebugPrintf("FPS: %ld\n", tickCount * DEBUG_PER_SECOND);

					debugTimer = execTimer;
					tickCount = 0;
				}

				#endif
			}
			else
			{
				#ifdef USE_PAUSE
				for (uint32_t i = 0; i < PAUSE_LOOP_NUM; i++)
				{
					_mm_pause();
				}
				#endif

				#ifdef USE_SLEEP
				Sleep(0);
				#endif
			}
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


