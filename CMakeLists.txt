cmake_minimum_required(VERSION 3.10)
project(SharedMemoryBus)

# Use C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories for headers
include_directories(include)

# Source files
set(SOURCES
    src/main.cpp
    src/circular_buffer.cpp
    src/shm_manager.cpp
)

# Create executable
add_executable(shared_memory_bus ${SOURCES})
