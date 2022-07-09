#include "TFrameTimer.h"

TFrameTimer::TFrameTimer()
{
	start = std::chrono::high_resolution_clock::now();
	end = std::chrono::high_resolution_clock::now();
}

double TFrameTimer::GetMilisecondsElapsed()
{
	if (m_IsRunning)
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

void TFrameTimer::ReStart()
{
	m_IsRunning = true;
	start = std::chrono::high_resolution_clock::now();
}

bool TFrameTimer::Stop()
{
	if (!m_IsRunning)
	{
		return false;
	}
	else
	{
		end = std::chrono::high_resolution_clock::now();
		m_IsRunning = true;
		return true;
	}
}

bool TFrameTimer::Start()
{
	if (!m_IsRunning)
	{
		start = std::chrono::high_resolution_clock::now();
		m_IsRunning = true;
		return true;
	}
	else
	{
		start = std::chrono::high_resolution_clock::now();
		m_IsRunning = true;
		return false;
	}
}
