#pragma once

#include <atomic>
#include <thread>
#include <cassert>
#include <cstdint>

constexpr size_t CACHELINE = 64;

template <typename T>
struct alignas(CACHELINE) Slot
{
    std::atomic<uint8_t> state;
    alignas(CACHELINE) T value;
};

enum SlotState : uint8_t
{
    EMPTY = 0,
    WRITING = 1,
    FULL = 2,
    READING = 3
};

template <typename T, size_t N>
struct alignas(CACHELINE) MmapQueue
{
    static_assert((N & (N - 1)) == 0, "N must be power of 2");

    std::atomic<size_t> head;
    char pad1[CACHELINE - sizeof(head)];
    std::atomic<size_t> tail;
    char pad2[CACHELINE - sizeof(tail)];

    Slot<T> slots[N];

    void init();
    bool enqueue(const T &item);
    bool dequeue(T &out);
};