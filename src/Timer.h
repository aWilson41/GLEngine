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

// Moving average timer causes drift over long intervals
// Try to recover over longer low variance intervals
// First we keep track of the amount of drift = (actual accumulated time - moving average accumulated time)
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

		// The averaging causes a loss from true time when timer is inconsistent
		// ie: If all 5 past values are the same there is no loss
		double dLoss = newDt - avgDt;
		loss += dLoss;

		// We plan to make this up over shorter payments. So when we have consistency loss -> 0
		avgDt += loss * 0.1;
		loss -= loss * 0.1;

		// If oldest dt is 0
		if (oldestDt == 0.0)
			return newDt;
		else
			return avgDt;
	}

	double getLoss() const { return loss; }

private:
	std::queue<double> dtQueue;
	double avgDt = 0.0;
	double loss = 0.0;
};