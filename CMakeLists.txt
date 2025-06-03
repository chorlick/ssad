cmake_minimum_required(VERSION 3.14)
project(ssad LANGUAGES CXX)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add source and include directories
include_directories(include)
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Main executable
add_executable(ssad ${SOURCES})

# Link system libraries if needed
target_link_libraries(ssad pthread)

# External JSON lib (header-only)
# You must install nlohmann/json or use FetchContent if desired
find_package(nlohmann_json REQUIRED)
target_link_libraries(ssad nlohmann_json::nlohmann_json)


# GoogleTest setup
enable_testing()
find_package(GTest REQUIRED)

# Unit test: Config
add_executable(test_config
    test/test_config.cpp
    src/config.cpp
    src/logger.cpp
)

target_include_directories(test_config PRIVATE include)
target_link_libraries(test_config GTest::GTest GTest::Main pthread)
add_test(NAME ConfigTest COMMAND test_config)
