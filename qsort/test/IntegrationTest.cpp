
#include <iostream>
#include <fstream>
#include <filesystem>
#include <random>
#include <array>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <thread>
#include "random.h"
#include "qsort.h"
namespace
{
	constexpr auto BigSize = 1000000;
	constexpr auto SmallSize = 10;
	constexpr auto Repeat = 10U;
}
TEST(PerformanceTest, QSortTestBigArray)
{
	UniformRandomGenerator generateVectorOfRandom;
	QSort qsort;
	auto startTime = std::chrono::high_resolution_clock::now();
	for (auto i = 0U; i < Repeat; ++i)
	{
		auto arr = generateVectorOfRandom.GenerateVectorOfRandom(0.0, 100.0, BigSize);
		EXPECT_TRUE(qsort.CheckSorting(arr) == "The array is not sorted");
		qsort.QuickSort(arr, 0, arr.size() - 1);
	}
	auto endtime = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed = (endtime - startTime) / Repeat;
	std::cout << "elapsed time: " << elapsed.count() << " seconds" << std::endl;
}

TEST(PerformanceTest, QSortTestSmallArray)
{
	UniformRandomGenerator generateVectorOfRandom;
	QSort qsort;
	auto startTime = std::chrono::high_resolution_clock::now();
	for (auto i = 0U; i < Repeat; ++i)
	{
		auto arr = generateVectorOfRandom.GenerateVectorOfRandom(0.0, 20.0, SmallSize);
		EXPECT_TRUE(qsort.CheckSorting(arr) == "The array is not sorted");
		qsort.QuickSort(arr, 0, arr.size() - 1);
	}
	auto endtime = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed = (endtime - startTime) / Repeat;
	std::cout << "elapsed time: " << elapsed.count() << " seconds" << std::endl;
}

TEST(SimplyTest, QSortAllValuesAreTheSame)
{
	std::vector<double> arr(SmallSize, 2.0);
	QSort qsort;
	qsort.QuickSort(arr, 0, arr.size() - 1);
	EXPECT_THAT(arr, ::testing::ElementsAreArray(std::vector<double>(SmallSize, 2.0)));
}

TEST(SimplyTest, QSortAllValuesAreTheSameBigArray)
{
	std::vector<double> arr(BigSize, 2.0);
	QSort qsort;
	qsort.QuickSort(arr, 0, arr.size() - 1);
	EXPECT_THAT(arr, ::testing::ElementsAreArray(std::vector<double>(BigSize, 2.0)));
}

TEST(SimplyTest, QSortSmallArray)
{
	QSort qsort;
	UniformRandomGenerator generateVectorOfRandom;
	auto arr = generateVectorOfRandom.GenerateVectorOfRandom(0.0, 20.0, SmallSize);
	EXPECT_TRUE(qsort.CheckSorting(arr) == "The array is not sorted");

	qsort.QuickSort(arr, 0, arr.size() - 1);
	EXPECT_TRUE(qsort.CheckSorting(arr) == "The array is sorted in ascending order");
}

TEST(SimplyTest, QSortBigArray)
{
	UniformRandomGenerator generateVectorOfRandom;
	auto arr = generateVectorOfRandom.GenerateVectorOfRandom(0.0, 100.0, BigSize);
	QSort qsort;
	EXPECT_TRUE(qsort.CheckSorting(arr) == "The array is not sorted");
	qsort.QuickSort(arr, 0, arr.size() - 1);
	EXPECT_TRUE(qsort.CheckSorting(arr) == "The array is sorted in ascending order");
}

TEST(PerformanceTest, QSortCompareTest)
{
	QSort qsort;
	UniformRandomGenerator generateVectorOfRandom;
	const auto inputArr = generateVectorOfRandom.GenerateVectorOfRandom(0.0, 20.0, SmallSize);
	EXPECT_TRUE(qsort.CheckSorting(inputArr) == "The array is not sorted");

	const auto spentTime = [&](std::function<void(std::vector<double>&, int, int)> func)
		{
			auto startTime = std::chrono::high_resolution_clock::now();
			for (auto i = 0U; i < Repeat; ++i)
			{
				auto arr = inputArr;
				func(arr, 0, arr.size() - 1);
			}
			auto endtime = std::chrono::high_resolution_clock::now();
			return std::chrono::duration<double>(endtime - startTime).count();
		};
	const auto simpleqsortTime = spentTime(&qsort.QuickSort);
	std::cout << "simpleqsortTime elapsed time: " << simpleqsortTime << " seconds" << std::endl;
	const auto threadqsortTime = spentTime(&qsort.ThreadedQuickSort);
	std::cout << "threadqsortTime elapsed time: " << threadqsortTime << " seconds" << std::endl;

	auto startTime = std::chrono::high_resolution_clock::now();
	for (auto i = 0U; i < Repeat; ++i)
	{
		auto arr = inputArr;
		std::qsort(arr.data(), arr.size(), sizeof(double), [](const void* a, const void* b)
			{
				return (*(double*)a > *(double*)b) ? 1 : (*(double*)a < *(double*)b) ? -1 : 0;
			});
	}
	auto endtime = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed = (endtime - startTime) / Repeat;
	const auto standardqsortTime = elapsed.count();
	std::cout << "standard qsort elapsed time: " << standardqsortTime << " seconds" << std::endl;
}

