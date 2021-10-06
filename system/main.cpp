#include "main.h"

#include "../math/vector4.h"

using namespace DragonLib;

#if defined(OS_WINDOWS)

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, TCHAR* lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER( hInstance );
	UNREFERENCED_PARAMETER( hPrevInstance );
	UNREFERENCED_PARAMETER( lpCmdLine );


	Window window;
	window.Initialize( WINDOW_WIDTH, WINDOW_HEIGHT );
	window.SetWindowShow( nCmdShow );

	RenderAPI renderer;
	renderer.Initialize(window);

	timeBeginPeriod(1);
	
	uint32_t tickCount = 0;
	Timer execTimer, debugTimer;
	execTimer.Start();
	debugTimer.Start();

	float len = 0;
	Vector4 vec1(1, 0, 0, 0);
	Vector4 vec2(0, 1, 0, 0);
	Vector4 vec3(0.0f);
	Vector4::Length(len, vec1);
	Vector4::Cross(vec3, vec2, vec1);

	DebugPrintf("Vector4_Length: %f\n", len);
	DebugPrintf("Vector4_Cross: %.1f, %.1f, %.1f, %.1f\n", vec3.x, vec3.y, vec3.z, vec3.w);


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
	renderer.Finalize();
	window.Finalize();

	return static_cast<int>(msg.wParam);
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


