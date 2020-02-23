#include "StdMultiThreader.h"

StdMultiThreader::StdMultiThreader()
{
#ifdef __APPLE__
	NumberOfThreads = 4;
#else
	NumberOfThreads = std::thread::hardware_concurrency();
#endif

	threadInfo = new ThreadInfo[NumberOfThreads];
}

StdMultiThreader::~StdMultiThreader()
{
	if (threadInfo != nullptr)
		delete[] threadInfo;
}

void StdMultiThreader::executeComplete()
{
	execute();
	synchronize();
}

void StdMultiThreader::execute()
{
	// Spawn all the threads
	threads = std::vector<std::thread>(NumberOfThreads);
	for (unsigned int i = 0; i < NumberOfThreads; i++)
	{
		threadInfo[i].NumberOfThreads = NumberOfThreads;
		threadInfo[i].ThreadID = i;
		threadInfo[i].UserData = data;
		threads[i] = std::thread(method, &threadInfo[i]);
	}
}

void StdMultiThreader::synchronize()
{
	// Wait for all the threads to finish
	for (unsigned int i = 0; i < NumberOfThreads; i++)
	{
		threads[i].join();
	}
}