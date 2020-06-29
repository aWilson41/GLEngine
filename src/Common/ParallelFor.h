#pragma once
#define NOMINMAX
#include "tbb/tbb.h"

template<class IndexType, class Function>
void parallelFor(const IndexType beginIdx, const IndexType endIdx, Function&& function)
{
	tbb::parallel_for(tbb::blocked_range<IndexType>(beginIdx, endIdx),
		[&](const tbb::blocked_range<IndexType>& r)
		{
			for (IndexType i = r.begin(), iEnd = r.end(); i < iEnd; ++i)
			{
				function(i);
			}
		});
}

template<class IndexType, class Function>
void parallelFor(const IndexType endIdx, Function&& function)
{
	tbb::parallel_for(tbb::blocked_range<IndexType>(static_cast<IndexType>(0), endIdx),
		[&](const tbb::blocked_range<IndexType>& r)
		{
			for (IndexType i = r.begin(), iEnd = r.end(); i < iEnd; ++i)
			{
				function(i);
			}
		});
}