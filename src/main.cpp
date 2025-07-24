#include <chrono>
#include <iostream>
#include <unistd.h>     // fork, getpid
#include <sys/wait.h>   // waitpid
#include <cstring>
#include "../include/shm_manager.hpp"
#include "../include/circular_buffer.hpp"

int main() {
    const char* shm_name = "/my_shm_benchmark";
    const size_t shm_size = sizeof(SharedMemoryHeader) 
                          + (MAX_MESSAGES * sizeof(MessageSlot))  
                          + (MAX_MESSAGES * sizeof(Message)); 

    const int iterations = 1'000'000;

    ShmManager shm(shm_name, shm_size, true);
    CircularBuffer buffer(shm.get_ptr());

    Message msg;
    std::memset(msg.data, 42, MAX_MESSAGE_SIZE);

    pid_t pid = fork();

    if (pid < 0) {
        std::cerr << "Fork failed\n";
        return 1;
    }
    else if (pid == 0) {
        // child process(reader)
        int read_count = 0;
        Message out;

        auto start = std::chrono::high_resolution_clock::now();

        while (read_count < iterations) {
            if (buffer.read(out) == BufferStatus::Success) {
                read_count++;
            }
        }

        auto end = std::chrono::high_resolution_clock::now();
        auto read_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

        std::cout << "[Child] Read " << read_count << " messages in " << read_ns << " ns, avg "
                  << (read_ns / (double)read_count) << " ns per read\n";

        return 0;
    }
    else {
        // parent process (writer)
        int write_count = 0;

        auto start = std::chrono::high_resolution_clock::now();

        while (write_count < iterations) {
            if (buffer.write(msg) == BufferStatus::Success) {
                write_count++;
            }
        }

        auto end = std::chrono::high_resolution_clock::now();
        auto write_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

        std::cout << "[Parent] Wrote " << write_count << " messages in " << write_ns << " ns, avg "
                  << (write_ns / (double)write_count) << " ns per write\n";

        waitpid(pid, nullptr, 0);

        return 0;
    }
}

