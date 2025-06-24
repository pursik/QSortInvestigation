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
	auto pivotElement = values[startIndex];
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

void QSort::ThreadedQuickSort(std::vector<double>& values, int startIndex, int endIndex)
{
	if (startIndex < endIndex)
	{
		auto partitioningIndex = Partition(values, startIndex, endIndex);

		std::thread leftThread;
		std::thread rightThread;

		if (endIndex - startIndex > limit)
		{
			leftThread = std::thread(&QSort::ThreadedQuickSort, ref(values), startIndex, partitioningIndex);
			rightThread = std::thread(&QSort::ThreadedQuickSort, ref(values), partitioningIndex + 1, endIndex);
		}
		else
		{
			QuickSort(values, startIndex, partitioningIndex);
			QuickSort(values, partitioningIndex + 1, endIndex);
		}

		if (leftThread.joinable())
		{
			leftThread.join();
		}

		if (rightThread.joinable())
		{
			rightThread.join();
		}
	}
}

std::string_view QSort::CheckSorting(const std::vector<double>& values) const
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