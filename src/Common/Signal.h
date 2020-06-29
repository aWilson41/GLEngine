#pragma once
#include <functional>
#include <memory>
#include <unordered_map>

template<typename... Args>
class Signal
{
public:
	template<typename T>
	int connect(T* instance, void (T::*func)(Args...))
	{
		return connect([=](Args... args) { (instance->*func)(args...); });
	}
	template<typename T>
	int connect(T* instance, void (T::*func)(Args...) const)
	{
		return connect([=](Args... args) { (instance->*func)(args...); });
	}

	unsigned int connect(const std::function<void(Args...)>& slot)
	{
		const unsigned int id = handleIter;
		slots.insert(std::make_pair(++handleIter, slot));
		return id;
	}

	void disconnect(int id)  {slots.erase(id); }

	void disconnectAll() { slots.clear(); }

	void emit(Args... p)
	{
		for (std::unordered_map<unsigned int, std::function<void(Args...)>>::iterator it = slots.begin(); it != slots.end(); it++)
		{
			it->second(p...);
		}
	}

private:
	std::unordered_map<unsigned int, std::function<void(Args...)>> slots;
	unsigned int handleIter = 0;
};