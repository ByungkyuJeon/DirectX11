#pragma once
#include <chrono>

class FrameTimer
{
public:
	FrameTimer();
	double GetMilisecondsElapsed();
	void ReStart();
	bool Stop();
	bool Start();

private:
	bool isrunning = false;
#ifdef _WIN32
	std::chrono::time_point<std::chrono::steady_clock> start;
	std::chrono::time_point<std::chrono::steady_clock> end;
#else
	std::chrono::time_point<std::chrono::system_clock> start;
	std::chrono::time_point<std::chrono::system_clock> end;
#endif
};