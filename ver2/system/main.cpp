// ------------------------------
// Author: Ryouma Kawashima
// Create: 2021/10/07
// Update: 2021/10/07
// ------------------------------
// Description
// 
// ------------------------------

#include <array>

#include "../utility/static_polymorphism.h"
#include "../utility/debug_printf.h"
#include "../utility/singleton.h"
#include "../utility/timer.h"
#include "../utility/parameter.h"

#include "../system/config.h"
#include "../system/system_timer.h"
#include "../system/manager.h"

using namespace DragonLib;

#if USE_WINDOWS

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, TCHAR* lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hInstance);
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(nCmdShow);

	Singleton<Manager>::Create();
	Singleton<SystemTimer>::Create();

	Manager* manager = Singleton<Manager>::GetInstance();
	manager->Initialize();

	SystemTimer* timer = Singleton<SystemTimer>::GetInstance();
	timer->ResetPhysicsTimer();
	timer->ResetUpdateTimer();
	timer->ResetDrawTimer();
	timer->ResetDebugTimer();

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
			bool waitFlag = true;

			if (timer->CheckPhysicsTimer())
			{
				do
				{
					manager->Physics();

				} while (timer->CheckPhysicsTimer());

				timer->ResetPhysicsTimer();
				waitFlag = false;
			}
			if (timer->CheckUpdateTimer())
			{
				manager->Update();
				timer->ResetUpdateTimer();
				waitFlag = false;
			}
			if (timer->CheckDrawTimer())
			{
				manager->Draw();
				timer->ResetDrawTimer();
				waitFlag = false;
			}
			#if defined( __DEBUG ) || defined( _DEBUG )
			if (timer->CheckDebugTimer())
			{
				manager->Debug();
				timer->ResetDebugTimer();
				waitFlag = false;
			}
			#endif

			if(waitFlag)
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

	manager->Finalize();

	Singleton<Timer>::Destroy();
	Singleton<Manager>::Destroy();

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