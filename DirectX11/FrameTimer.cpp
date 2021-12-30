#include "FrameTimer.h"

FrameTimer::FrameTimer()
{
	start = std::chrono::high_resolution_clock::now();
	end = std::chrono::high_resolution_clock::now();
}

double FrameTimer::GetMilisecondsElapsed()
{
	if (isrunning)
	{
		auto elapsed = std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - start);
		return elapsed.count();
	}
	else
	{
		auto elapsed = std::chrono::duration<double, std::milli>(end - start);
		return elapsed.count();
	}
}

void FrameTimer::ReStart()
{
	isrunning = true;
	start = std::chrono::high_resolution_clock::now();
}

bool FrameTimer::Stop()
{
	if (!isrunning)
	{
		return false;
	}
	else
	{
		end = std::chrono::high_resolution_clock::now();
		isrunning = true;
		return true;
	}
}

bool FrameTimer::Start()
{
	if (!isrunning)
	{
		start = std::chrono::high_resolution_clock::now();
		isrunning = true;
		return true;
	}
	else
	{
		start = std::chrono::high_resolution_clock::now();
		isrunning = true;
		return false;
	}
}
