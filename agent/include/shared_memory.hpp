#pragma once

#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdexcept>
#include <cstring>
#include <string>

template <typename T>
class SharedMemory
{
public:
    SharedMemory(const std::string &path, bool create);
    ~SharedMemory();
    T *get() const;

private:
    int fd;
    T *data;
    size_t size;
};