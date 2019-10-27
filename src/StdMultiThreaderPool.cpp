#include "StdMultiThreaderPool.h"

static void startThreadWait(void* data, ThreadInfo* threadInfo)
{
	static_cast<StdMultiThreaderPool*>(data)->threadWait(threadInfo);
}

StdMultiThreaderPool::StdMultiThreaderPool()
{
#ifdef __APPLE__
	NumberOfThreads = 4;
#else
	NumberOfThreads = std::thread::hardware_concurrency();
#endif

	threadInfo = new ThreadInfo[NumberOfThreads];
	workReady.resize(NumberOfThreads);
	std::fill_n(workReady.begin(), workReady.size(), false);
}

StdMultiThreaderPool::~StdMultiThreaderPool()
{
	stop();
	if (threadInfo != nullptr)
		delete[] threadInfo;
}

void StdMultiThreaderPool::start()
{
	// Spawn all the threads in waiting
	threads = std::vector<std::thread>(NumberOfThreads);
	for (unsigned int i = 0; i < NumberOfThreads; i++)
	{
		threadInfo[i].NumberOfThreads = NumberOfThreads;
		threadInfo[i].ThreadID = i;
		threads[i] = std::thread(startThreadWait, this, &threadInfo[i]);
	}
}

void StdMultiThreaderPool::execute()
{
	// Start the jobs
	{
		std::lock_guard<std::mutex> lock(StartThreadMutex);
		threadCounter = NumberOfThreads;
		std::fill_n(workReady.begin(), workReady.size(), true);
	}
	waitForWorkCondition.notify_all();

	// Wait for all the workers to complete (threads decrement the counter)
	{
		std::unique_lock<std::mutex> lock(StopThreadMutex);
		waitForWorkersCondition.wait(lock, [this](){ return threadCounter == 0; });
	}
}

void StdMultiThreaderPool::stop()
{
	done = true;
	waitForWorkCondition.notify_all();

	// Wait for all the threads to finish
	for (unsigned int i = 0; i < NumberOfThreads; i++)
	{
		threads[i].join();
	}
}

void StdMultiThreaderPool::threadWait(ThreadInfo* threadInfo)
{
	// Processes all jobs sent to this thread until shutdown
	while (!done)
	{
		// Wait for work to do
		{
			std::unique_lock<std::mutex> lock(StartThreadMutex);
			waitForWorkCondition.wait(lock, [this, threadInfo]() { return done || workReady[threadInfo->ThreadID]; });
		}

		// Do the work
		threadInfo->UserData = data;
		method(threadInfo);

		// Decrement notify main thread of completion
		{
			std::lock_guard<std::mutex> lock(StopThreadMutex);
			threadCounter--;
			workReady[threadInfo->ThreadID] = false;
		}
		waitForWorkersCondition.notify_all();
	}
}