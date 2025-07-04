#include "../include/shared_memory.hpp"

template <typename T>
SharedMemory<T>::SharedMemory(const std::string &path, bool create) 
    : fd(-1), data(nullptr), size(sizeof(T))
{
    int flags = create ? (O_CREAT | O_RDWR) : O_RDWR;
    fd = open(path.c_str(), flags, 0666);
    if (fd == -1)
    {
        throw std::runtime_error("Failed to open shared memory file: " + path + " err: " + strerror(errno));
    }

    if (create)
    {
        if (ftruncate(fd, size) == -1)
        {
            close(fd);
            throw std::runtime_error("Failed to set size of shared memory file");
        }
    }

    void *addr = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED)
    {
        close(fd);
        throw std::runtime_error("mmap failed: " + std::string(strerror(errno)));
    }

    data = static_cast<T *>(addr);
}

template <typename T>
SharedMemory<T>::~SharedMemory()
{
    if (data)
    {
        munmap(data, size);
        data = nullptr;
    }
    if (fd != -1)
    {
        close(fd);
        fd = -1;
    }
}

template <typename T>
T *SharedMemory<T>::get() const { return data; }