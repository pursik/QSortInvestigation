#include <vector>
#include <string>

class QSort
{
public:
	// Partition the array and get the index of the pivot element
	static int Partition(std::vector<double>& values, int startIndex, int endIndex);

	// Recursively sort the subarray
	static void QuickSort(std::vector<double>& values, int startIndex, int endIndex);

	// Recursively sort the subarray using threads
	static void ThreadedQuickSort(std::vector<double>& values, int startIndex, int endIndex);

	static std::string_view CheckSorting(const std::vector<double>& values);
};
