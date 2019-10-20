#pragma once
#include <thread>
#include <vector>
#include <functional>

class ThreadInfo
{
public:
	int ThreadID;
	int NumberOfThreads;
	void* UserData = nullptr;
};

class StdMultiThreader
{
public:
	StdMultiThreader();
	~StdMultiThreader();

	// Executes the function specified on the number of threads specified
	// Waits for completion
	void executeComplete();

	// Executes the function specified on the number of threads specified
	// Does not wait for completion
	void execute();

	void setMethod(std::function<void(ThreadInfo*)> method, void* data);

	void setNumberOfThreads(unsigned int numThreads) { NumberOfThreads = numThreads; }
	unsigned int getNumberOfThreads() { return NumberOfThreads; }

	// Waits for all the threads to finish
	void synchronize();

protected:
	std::vector<std::thread> threads;
	unsigned int NumberOfThreads = 1;
	ThreadInfo* threadInfo = nullptr;

	std::function<void(ThreadInfo*)> method;
	void* data = nullptr;
};