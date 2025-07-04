#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <thread>
#include <atomic>
#include <csignal>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <poll.h>
#include <cstring>
#include <cerrno>

#include "aho_corasick.hpp"
#include "mmap_queue.hpp"
#include "shared_memory.hpp"



constexpr size_t QUEUE_SIZE = 16384;
constexpr size_t TEXT_SIZE = 256;

struct RawEvent {
    uint8_t type; // 0 = SYSLOG_LINE, 1 = USB_EVENT, 2 = FILE_DELETE
    uint64_t event_id;
    char text[TEXT_SIZE];
};
using QueueType = MmapQueue<RawEvent, QUEUE_SIZE>;

std::atomic<bool> g_running(true);
std::atomic<uint64_t> g_event_counter(0);

int main() {
    std::cout << "Agent starting...\n";
    SharedMemory<QueueType> shm("./tmp/event_queue_shm", true);
    QueueType* queue = shm.get();
    queue->init();

    std::cout << "Agent stopped.\n";
    return 0;
}