#pragma once
// -----------------------------------
// Author: Ryouma Kawashima
// Create: 2021/10/04
// Update: 2021/10/04
// -----------------------------------
#include <chrono>

namespace DragonLib
{
	class Timer
	{
	public:
		// Durations
		typedef std::chrono::nanoseconds	NanoSeconds;	// 10 ^ -9
		typedef std::chrono::microseconds	MicroSeconds;	// 10 ^ -6
		typedef std::chrono::milliseconds	MilliSeconds;	// 10 ^ -3
		typedef std::chrono::seconds		Seconds;		// 1
		typedef std::chrono::minutes		Minutes;		// 60
		typedef std::chrono::hours			Hours;			// 360

	public:
		virtual void Start()
		{
			StartTime = std::chrono::high_resolution_clock::now();
		}
		virtual void Stop()
		{
			EndTime = std::chrono::high_resolution_clock::now();
		}

		template <class Duration>
		uint64_t GetElapsedTime()
		{
			return std::chrono::duration_cast<Duration>(EndTime - StartTime).count();
		}

	protected:
		std::chrono::high_resolution_clock::time_point StartTime;
		std::chrono::high_resolution_clock::time_point EndTime;
	};

}
