#include "../include/circular_buffer.hpp"
#include <cstring>

CircularBuffer::CircularBuffer(void* shm_base) {
    header_ = reinterpret_cast<SharedMemoryHeader*>(shm_base);
    slots_ = reinterpret_cast<MessageSlot*>(static_cast<char*>(shm_base) + sizeof(SharedMemoryHeader));
    messages_ = reinterpret_cast<Message*>(static_cast<char*>(shm_base) + sizeof(SharedMemoryHeader) + sizeof(MessageSlot) * MAX_MESSAGES);
}

BufferStatus CircularBuffer::read(Message& out) {
    int head = header_->head.value.load(std::memory_order_relaxed);

    if (head == header_->tail.value.load(std::memory_order_acquire)) {
        return BufferStatus::Empty;
    }

    if (!slots_[head].ready.load(std::memory_order_acquire)) {
        return BufferStatus::Empty;
    }

    std::memcpy(out.data, messages_[head].data, MAX_MESSAGE_SIZE);
    slots_[head].ready.store(false, std::memory_order_release);

    int next_head = (head + 1) % MAX_MESSAGES;
    header_->head.value.store(next_head, std::memory_order_release);

    return BufferStatus::Success;
}


BufferStatus CircularBuffer::write(const Message& msg) {
    int tail = header_->tail.value.load(std::memory_order_relaxed);
    int next_tail = (tail + 1) % MAX_MESSAGES;

    if (next_tail == header_->head.value.load(std::memory_order_acquire)) {
        return BufferStatus::Full;
    }

    if (slots_[tail].ready.load(std::memory_order_acquire)) {
        return BufferStatus::Full;
    }

    std::memcpy(messages_[tail].data, msg.data, MAX_MESSAGE_SIZE);
    slots_[tail].ready.store(true, std::memory_order_release);

    header_->tail.value.store(next_tail, std::memory_order_release);
    return BufferStatus::Success;
}
