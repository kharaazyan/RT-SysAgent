#include "../include/mmap_queue.hpp"

template <typename T, size_t N>
void MmapQueue<T, N>::init()
{
    head.store(0, std::memory_order_relaxed);
    tail.store(0, std::memory_order_relaxed);
    for (size_t i = 0; i < N; ++i)
        slots[i].state.store(EMPTY, std::memory_order_relaxed);
}

template <typename T, size_t N>
bool MmapQueue<T, N>::enqueue(const T &item)
{
    for (int i = 0; i < 10000; ++i)
    {
        size_t pos = tail.fetch_add(1, std::memory_order_acq_rel) & (N - 1);
        auto &slot = slots[pos];

        uint8_t expected = EMPTY;
        if (slot.state.compare_exchange_strong(expected, WRITING, std::memory_order_acquire))
        {
            slot.value = item;
            slot.state.store(FULL, std::memory_order_release);
            return true;
        }
        std::this_thread::yield();
    }
    return false;
}

template <typename T, size_t N>
bool MmapQueue<T, N>::dequeue(T &out)
{
    for (int i = 0; i < 10000; ++i)
    {
        size_t pos = head.fetch_add(1, std::memory_order_acq_rel) & (N - 1);
        auto &slot = slots[pos];

        uint8_t expected = FULL;
        if (slot.state.compare_exchange_strong(expected, READING, std::memory_order_acquire))
        {
            out = slot.value;
            slot.state.store(EMPTY, std::memory_order_release);
            return true;
        }
        std::this_thread::yield();
    }
    return false;
}