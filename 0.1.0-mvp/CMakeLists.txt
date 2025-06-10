cmake_minimum_required(VERSION 3.20)
project(ElevatorSystem)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set the vcpkg toolchain file
set(CMAKE_TOOLCHAIN_FILE "C:/Users/I.Nikiforov/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")

# Find required packages
find_package(GTest CONFIG REQUIRED)

# Create a custom target that compiles main.cpp with a preprocessor flag to exclude main
# This allows us to use the same source file for both the main executable and tests
add_library(elevator_lib OBJECT main.cpp)
target_compile_definitions(elevator_lib PRIVATE EXCLUDE_MAIN=1)
target_include_directories(elevator_lib PRIVATE .)

# Add the main executable (includes main function)
add_executable(elevator_system main.cpp)
target_include_directories(elevator_system PRIVATE .)

# Add the test executable
add_executable(elevator_system_tests 
    elevator_system_tests.cpp
    $<TARGET_OBJECTS:elevator_lib>  # Use the object library without main
)

# Link gtest and gmock to the test executable
target_link_libraries(elevator_system_tests 
    GTest::gtest 
    GTest::gtest_main 
    GTest::gmock 
    GTest::gmock_main
)

# Include directories
target_include_directories(elevator_system_tests PRIVATE .)

# Enable testing
enable_testing()
add_test(NAME ElevatorTests COMMAND elevator_system_tests)

