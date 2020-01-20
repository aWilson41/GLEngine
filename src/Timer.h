#pragma once
#include <chrono>
#include <queue>

class Time
{
public:
	using S = std::ratio<1>;
	using MS = std::milli;
};

template<class Period = Time::S>
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

protected:
	std::chrono::time_point<std::chrono::steady_clock> begin;
	std::chrono::time_point<std::chrono::steady_clock> end;
};

template<unsigned int N = 5, class Period = Time::S>
class MovingAverageTimer : public Timer<Period>
{
public:
	MovingAverageTimer()
	{
		for (unsigned int i = 0; i < N; i++)
		{
			dtQueue.push(0.0);
		}
	}

public:
	double getElapsedAvg()
	{
		// Sample the dt
		end = std::chrono::steady_clock::now();
		const double newDt = std::chrono::duration<double, Period>(end - begin).count();
		// Put the new one at the back
		dtQueue.push(newDt);

		// Remove the oldest one from the front
		const double oldestDt = dtQueue.front();
		dtQueue.pop();

		// Advance the moving average
		avgDt = (avgDt * N - oldestDt + newDt) / N;

		// If oldest dt is 0
		if (oldestDt == 0.0)
			return newDt;
		else
			return avgDt;
	}

private:
	std::queue<double> dtQueue;
	double avgDt = 0.0;
};