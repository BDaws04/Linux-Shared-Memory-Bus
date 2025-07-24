# Shared Memory Circular Buffer Benchmark

A high-performance circular buffer using POSIX shared memory (`shm_open`) for low-latency inter-process communication (IPC) on Linux.

### Compiling and Building

Make sure you have a C++17 compatible compiler installed (e.g., `g++` or `clang++`) along with CMake (≥ 3.10).

### Using g++

From the project root directory, run:

```bash
g++ -std=c++17 -pthread -o main src/main.cpp src/circular_buffer.cpp src/shm_manager.cpp
```

```CMake
mkdir build
cd build
cmake ..
make
```

### Benchmark:
Computer Specs: 
- CPU: Intel(R) Core(TM) i5-8250U CPU @ 1.60GHz
- RAM: 8GB
- Compiler: g++ version 15.1.0

Performance:
- Time taken varied between 80-190ns, performance below based on 100 runs 
- 1,000,000 messages written and read in ~120 milliseconds
- Average ~120 ns per message (write and read)

### Project Architecture

This project implements a shared memory circular buffer designed for fast message passing between processes on Linux

### Core Design

- ShmManager sets up and manages POSIX shared memory regions with proper lifecycle handling.
- CircularBuffer encapsulates a ring buffer structure with a header for metadata and a fixed number of message slots.
- Messages are fixed-size chunks written and read sequentially, supporting zero-copy shared memory IPC.
- Synchronization is done without heavy locking to maximize throughput and minimize latency.

### How Orders Are Processed

- The main program forks into two processes:
- Parent writes a fixed number of messages into the buffer.
- Child reads the same number of messages from the buffer.
- Both measure their individual latency in nanoseconds.
- The circular buffer uses head/tail indices and wrap-around logic to manage slots.
- 
### Performance Highlights

- Utilizes POSIX shared memory (shm_open) for efficient inter-process communication.
- Avoids dynamic memory allocation during runtime, improving predictability.
- Simple ring buffer with fixed-size messages ensures cache-friendly memory access.
- Lock-free or minimal-lock synchronization reduces contention and overhead.

### Summary
This shared memory circular buffer benchmark demonstrates how to achieve low-latency IPC on Linux with a clean and simple design, suitable for high-performance applications needing fast process-to-process communication.
