#include "../utility/debug_printf.h"
#include "../utility/static_polymorphism.h"
#include "../utility/singleton.h"
#include "../utility/timer.h"

#include "../utility/parameter.h"
#include "../system/config.h"

#include "manager.h"

#include <cassert>

using namespace DragonLib;

void Manager::Initialize()
{
	Singleton<WindowAPI>::Create();
	Singleton<RenderAPI>::Create();

	m_WindowAPI = Singleton<WindowAPI>::GetInstance();
	m_RenderAPI = Singleton<RenderAPI>::GetInstance();

	m_WindowAPI->Initialize(WINDOW_WIDTH, WINDOW_HEIGHT);
	m_RenderAPI->Initialize(m_WindowAPI);

	m_WindowAPI->Show(5);
	m_WindowAPI->Update();
}
void Manager::Finalize()
{
	m_RenderAPI->Finalize();
	m_WindowAPI->Finalize();
}
void Manager::Physics()
{

}
void Manager::Update()
{

}
void Manager::Draw()
{

}
void Manager::Debug()
{
	#if defined( __DEBUG ) || defined( _DEBUG )

	// FPS‚ÌŒv‘ª
	//if (execTimer.GetStartTime<NanoSeconds>() >
	//	debugTimer.GetStartTime<NanoSeconds>() + DEBUG_WAIT_TIME)
	//{
	//	DebugPrintf("FPS: %ld\n", tickCount * DEBUG_PER_SECOND);
	//
	//	debugTimer = execTimer;
	//	tickCount = 0;
	//}

	#endif
}
