
#include <iostream>
#include <fstream>
#include <filesystem>
#include <random>
#include <array>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <thread>
#include <chrono>
#include <numeric>
#include "random.h"
#include "qsort.h"
namespace
{
	double SpentTime(const std::vector<double>& inputArr, const std::function<void(std::vector<double>&, int, int)>& func, std::uint32_t repeat)
	{
		using clock = std::chrono::high_resolution_clock;
		double totalTime = 0.0;

		for (auto i = 0U; i < repeat; ++i)
		{
			auto arr = inputArr;
			auto startTime = clock::now();
			func(arr, 0, arr.size() - 1);
			auto endTime = clock::now();
			EXPECT_TRUE(QSort::CheckSorting(arr) == "The array is sorted in ascending order");
			totalTime += std::chrono::duration<double>(endTime - startTime).count();
		}
		return totalTime / repeat;
	}

	double SpentTime(const std::vector<double>& inputArr, std::uint32_t repeat)
	{
		using clock = std::chrono::high_resolution_clock;
		double totalTime = 0.0;

		for (auto i = 0U; i < repeat; ++i)
		{
			auto arr = inputArr;
			auto startTime = clock::now();
			std::qsort(arr.data(), arr.size(), sizeof(double), [](const void* a, const void* b) -> int
				{
					double lhs = *static_cast<const double*>(a);
					double rhs = *static_cast<const double*>(b);
					return (lhs > rhs) - (lhs < rhs);
				});
			auto endTime = clock::now();
			EXPECT_TRUE(QSort::CheckSorting(arr) == "The array is sorted in ascending order");
			totalTime += std::chrono::duration<double>(endTime - startTime).count();
		}
		return totalTime / repeat;
	}

	auto create_random_array(std::uint32_t size, std::pair<double, double> range)
	{
		UniformRandomGenerator generator;
		return generator.GenerateVectorOfRandom(range.first, range.second, size);
	}

	auto create_sorted_array(std::uint32_t size)
	{
		std::vector<double> arr(size);
		std::iota(arr.begin(), arr.end(), 0.0);
		return arr;
	}

	auto create_uniform_array(std::uint32_t size)
	{
		return std::vector<double>(size, 2.0);
	}
}
namespace qsortinvestigation
{
	struct QSortTestParam
	{
		std::vector<double> array;
		std::string fileName = "elapsed_time.txt";
	};
	class QSortTestSuitFixture : public ::testing::TestWithParam<QSortTestParam>
	{
	};
	const std::vector<QSortTestParam> TestParameters = {
		{create_random_array(2,{0.0,100.0}), "elapsed_time_random.txt"},
		{create_random_array(10,{0.0,100.0}), "elapsed_time_random.txt"},
		{create_random_array(1000,{0.0,100.0}), "elapsed_time_random.txt"},
		{create_random_array(1000000,{0.0,100.0}), "elapsed_time_random.txt"},
		{create_sorted_array(2), "elapsed_time_sorted.txt"},
		{create_sorted_array(10),"elapsed_time_sorted.txt"},
		{create_sorted_array(1000),"elapsed_time_sorted.txt"},
		{create_sorted_array(1000000),"elapsed_time_sorted.txt"},
		{create_uniform_array(2),"elapsed_time_uniform.txt"},
		{create_uniform_array(10),"elapsed_time_uniform.txt"},
		{create_uniform_array(1000),"elapsed_time_uniform.txt"},
		{create_uniform_array(1000000),"elapsed_time_uniform.txt"},
	};
	TEST_P(QSortTestSuitFixture, QSortSpentTimeTest)
	{
		const auto arr = GetParam().array;
		ASSERT_TRUE(!arr.empty());

		const auto arrSize = arr.size();

		auto filename = GetParam().fileName;
		std::ofstream out(filename, std::ios::app);

		const auto elapsedTimeCollect = [&](std::uint32_t repeat)
			{
				out << "repeat = " << repeat << "  array size = "
					<< arrSize << " user qsort time = "
					<< SpentTime(arr, &QSort::QuickSort, repeat) << " threaded qsort time = "
					<< SpentTime(arr, &QSort::ThreadedQuickSort, repeat) << " standart qsort time = "
					<< SpentTime(arr, repeat) << '\n';
			};
		elapsedTimeCollect(1);
		elapsedTimeCollect(100);
		out.close();
	}
	INSTANTIATE_TEST_SUITE_P(
		QSortTestSuit, QSortTestSuitFixture, ::testing::ValuesIn(TestParameters));
}





