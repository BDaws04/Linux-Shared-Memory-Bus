#ifndef MESSAGE_H
#define MESSAGE_H

#include <cstdint>
#include <atomic>
#include <cstddef>

constexpr size_t MAX_MESSAGE_SIZE = 256;
constexpr size_t MAX_MESSAGES = 1024;

template<typename T>
struct alignas(64) PaddedAtomic {
    std::atomic<T> value;
    char padding[64 - sizeof(std::atomic<T>)];
};

struct SharedMemoryHeader {
    PaddedAtomic<int> ref_count;
    PaddedAtomic<int> head;
    PaddedAtomic<int> tail;
};
static_assert(sizeof(SharedMemoryHeader) == 64 * 3, "SharedMemoryHeader size() must be 192 bytes");

struct Message {
    uint8_t size;
    char size_padding[3];
    char data[252];
};
static_assert(sizeof(Message) == MAX_MESSAGE_SIZE, "Message size() must be 256 bytes");

#endif