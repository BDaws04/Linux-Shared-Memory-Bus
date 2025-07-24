#ifndef CIRCULAR_BUFFER_HPP
#define CIRCULAR_BUFFER_HPP

#include "message.hpp"

enum class BufferStatus {
    Success,
    Empty,
    Full,
    Error
};

class CircularBuffer {

    public:
        CircularBuffer(void* shm_base);
        ~CircularBuffer() = default;

        CircularBuffer(const CircularBuffer&) = delete;
        CircularBuffer& operator=(const CircularBuffer&) = delete;

        CircularBuffer(CircularBuffer&&) = delete;             
        CircularBuffer& operator=(CircularBuffer&&) = delete;


        BufferStatus read(Message& message);
        BufferStatus write(const Message& message);


    private:
        SharedMemoryHeader* header_;
        MessageSlot* slots_;
        Message* messages_;
};

#endif