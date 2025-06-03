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