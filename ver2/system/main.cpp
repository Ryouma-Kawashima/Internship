// ------------------------------
// Author: Ryouma Kawashima
// Create: 2021/10/07
// Update: 2021/10/07
// ------------------------------
// Description
// 
// ------------------------------


#include "../utility/debug_printf.h"
#include "../utility/static_polymorphism.h"
#include "../utility/parameter.h"
#include "../system/config.h"
#include "../utility/timer.h"

#include "../window/window_api.h"
#include "../render/render_api.h"

#include <cassert>

using namespace DragonLib;

#if USE_WINDOWS

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, TCHAR* lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hInstance);
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	WindowAPI window;
	RenderAPI render;

	window.Initialize( WINDOW_WIDTH, WINDOW_HEIGHT );
	render.Initialize();

	window.Show(nCmdShow);
	window.Update();

	
	uint32_t tickCount = 0;
	Timer execTimer, debugTimer;
	execTimer.Start();
	debugTimer.Start();

	timeBeginPeriod(1);

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

			if (execTimer.GetElapsedTime<NanoSeconds>() > EXEC_WAIT_TIME)
			{
				execTimer.Start();
				tickCount++;

				// FPS‚ÌŒv‘ª
				#if defined( __DEBUG ) || defined( _DEBUG )

				if (execTimer.GetStartTime<NanoSeconds>() > 
					debugTimer.GetStartTime<NanoSeconds>() + DEBUG_WAIT_TIME)
				{
					DebugPrintf("FPS: %ld\n", tickCount * DEBUG_PER_SECOND);

					debugTimer = execTimer;
					tickCount = 0;
				}

				#endif


				render.Begin();
				render.End();

			}
			else
			{
				#if USE_PAUSE
				for (uint32_t i = 0; i < PAUSE_LOOP_NUM; i++)
				{
					_mm_pause();
				}
				#endif

				#if USE_SLEEP
				Sleep(0);
				#endif
			}
		}
	}

	timeEndPeriod(1);
	
	render.Finalize();
	window.Finalize();

	return 0;
}

#elif USE_LINUX

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