#include "qsort.h"
#include <thread>
#include <algorithm>

namespace
{
	constexpr int limit = 1000;
}

void QSort::QuickSort(std::vector<double>& values, int startIndex, int endIndex)
{
	if (startIndex < endIndex)
	{
		auto pivotIndex = Partition(values, startIndex, endIndex);
		// Recursively sort the part before the partition index
		QuickSort(values, startIndex, pivotIndex);
		// Recursively sort the part after the partition index
		QuickSort(values, pivotIndex + 1, endIndex);
	}
}

int QSort::Partition(std::vector<double>& values, int startIndex, int endIndex)
{
	auto pivotElement = values[(startIndex + endIndex) / 2];//values[startIndex];
	auto i = startIndex - 1;
	auto j = endIndex + 1;

	while (true)
	{
		while (++i <= endIndex && values[i] < pivotElement) {}

		while (--j >= startIndex && values[j] > pivotElement) {}

		// If indexes have crossed, is complete
		if (i >= j)
		{
			return j;
		}
		//move elements at i and j to the opposit partition
		std::swap(values[i], values[j]);
	}
}

std::string_view QSort::CheckSorting(const std::vector<double>& values)
{
	if (std::is_sorted(values.begin(), values.end()))
	{
		return "The array is sorted in ascending order";
	}
	else if (std::is_sorted(values.rbegin(), values.rend())) // from the end to the beginning
	{
		return "The array is sorted in decreasing order";
	}
	else
	{
		return "The array is not sorted";
	}
}

void QSort::ThreadedQuickSort(std::vector<double>& values, int startIndex, int endIndex)
{
	if (startIndex >= endIndex)
	{
		return;
	}
	static const unsigned maxThreads = std::thread::hardware_concurrency();
	static std::atomic<unsigned> activeThreads{ 1 };
	auto partitioningIndex = Partition(values, startIndex, endIndex);

	std::thread leftPartThread;
	std::thread rightPartThread;

	if (endIndex - startIndex > limit)
	{
		if (activeThreads.load() < maxThreads)
		{
			activeThreads.fetch_add(1);

			leftPartThread = std::thread([&] {
				ThreadedQuickSort(values, startIndex, partitioningIndex);
				activeThreads.fetch_sub(1);
				});
		}
		if (activeThreads.load() < maxThreads)
		{
			activeThreads.fetch_add(1);

			rightPartThread = std::thread([&] {
				ThreadedQuickSort(values, partitioningIndex + 1, endIndex);
				activeThreads.fetch_sub(1);
				});
		}
	}
	if (leftPartThread.joinable())
	{
		leftPartThread.join();
	}
	else
	{
		QuickSort(values, startIndex, partitioningIndex);
	}

	if (rightPartThread.joinable())
	{
		rightPartThread.join();
	}
	else
	{
		QuickSort(values, partitioningIndex + 1, endIndex);
	}
}


