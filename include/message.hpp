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
    uint32_t size;
    char padding[64 - sizeof(std::atomic<T>) - sizeof(uint32_t)];
};

struct SharedMemoryHeader {
    PaddedAtomic<int> ref_count;
    PaddedAtomic<int> head;
    PaddedAtomic<int> tail;
};
static_assert(sizeof(SharedMemoryHeader) == 64 * 3, "SharedMemoryHeader size() must be 192 bytes");

struct alignas(64) MessageSlot {
    std::atomic<bool> ready;        
    char padding[64 - sizeof(std::atomic<bool>)];
};
static_assert(sizeof(MessageSlot) == 64, "MessageSlot size() must be 64 bytes");

struct Message {
    char data[MAX_MESSAGE_SIZE];
};

#endif