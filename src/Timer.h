#pragma once
#include <chrono>

using SECS = std::ratio<1>;
using MS = std::milli;

template<class Period = SECS>
class Timer
{
public:
	void start() { begin = std::chrono::steady_clock::now(); }
	void stop() { end = std::chrono::steady_clock::now(); }
	double getElapsed()
	{
		end = std::chrono::steady_clock::now();
		return std::chrono::duration<double, Period>(end - begin).count();
	}
	void reset() { start(); }

private:
	std::chrono::time_point<std::chrono::steady_clock> begin;
	std::chrono::time_point<std::chrono::steady_clock> end;
};