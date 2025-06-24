# Specify the minimum version of CMake required
cmake_minimum_required(VERSION 3.15)

# Define the project name and version
project(dataexchange VERSION 1.0 LANGUAGES CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include Conan's toolchain and dependencies
include(${CMAKE_BINARY_DIR}/conan_toolchain.cmake)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/qsort/include)

find_package(GTest REQUIRED)
find_package(Boost REQUIRED COMPONENTS program_options)


add_executable(qsortapp
   "qsort/main.cpp"
     "qsort/src/qsort.cpp")
target_include_directories(qsortapp PRIVATE "include")

# Link libraries
target_link_libraries(qsortapp
	PRIVATE
	Boost::program_options
)
# Enable testing and include the GoogleTest module
enable_testing()
include(GoogleTest)

add_executable(test_qsort 
"qsort/test/IntegrationTest.cpp"
"qsort/src/qsort.cpp"
)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/qsort/include)
target_link_libraries(test_qsort
    PRIVATE
    GTest::gtest_main
)
gtest_discover_tests(test_qsort)
