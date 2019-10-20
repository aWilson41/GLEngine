#pragma once
#include <atomic>
#include <functional>
#include <mutex>
#include <thread>
#include <vector>

class ThreadInfo
{
public:
	int ThreadID;
	int NumberOfThreads;
	void* UserData = nullptr;
};

// Simple thread pool implementation. Waits for all threads to finish but does not
// close the workers
class StdMultiThreaderPool
{
public:
	StdMultiThreaderPool();
	~StdMultiThreaderPool();

	// Starts the threads
	void start();

	// Submits job to each thread calling method
	void execute();

	// Waits for all threads to finish
	void stop();

	void threadWait(ThreadInfo* threadInfo);

	// Set the method to complete en-batch
	void setMethod(std::function<void(ThreadInfo*)> method, void* data);
	void setNumberOfThreads(unsigned int numThreads) { NumberOfThreads = numThreads; }

	unsigned int getNumberOfThreads() { return NumberOfThreads; }
	int getNumOfThreads() { return NumberOfThreads; }

protected:
	// For threading
	std::function<void(ThreadInfo*)> method;
	std::vector<std::thread> threads;
	unsigned int NumberOfThreads = 1;
	void* data = nullptr;
	ThreadInfo* threadInfo = nullptr;

	// For thread communication
	std::mutex StartThreadMutex;
	std::mutex StopThreadMutex;
	std::atomic<int> threadCounter;
	std::condition_variable waitForWorkCondition;
	std::condition_variable waitForWorkersCondition;
	std::vector<bool> workReady;
	bool done = false;
};