#pragma once
#include <cstddef>

namespace attic {
namespace gpu {

// GPU memory management
class MemoryPool {
public:
    MemoryPool();
    ~MemoryPool();
    
    bool initialize(size_t initial_size);
    void shutdown();
    
    void* allocate(size_t size);
    void deallocate(void* ptr);
    
    void* allocate_host(size_t size);
    void deallocate_host(void* ptr);
    
    void copy_host_to_device(void* dst, const void* src, size_t size);
    void copy_device_to_host(void* dst, const void* src, size_t size);
    void copy_device_to_device(void* dst, const void* src, size_t size);
    
private:
    class Impl;
    Impl* impl_;
};

} // namespace gpu
} // namespace attic
