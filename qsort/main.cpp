#include "qsort.h"
#include "random.h"
#include <iostream>

int main(int argc, const char* argv[])
{
	try
	{
		UniformRandomGenerator generateVectorOfRandom;
		QSort qsort;
		auto arr = generateVectorOfRandom.GenerateVectorOfRandom(0.0, 100.0, 1000000);
		qsort.QuickSort(arr, 0, arr.size() - 1);
	}
	catch (const std::exception& e)
	{
		std::cerr << "Standard exception: " << e.what() << std::endl;
		return -1;
	}
	catch (...)
	{
		std::cerr << "Unknown exception occurred." << std::endl;
		return -1;
	}
	return 0;
}
