#pragma once
#include <cstddef>

namespace attic {
namespace gpu {

// GPU device abstraction
class Device {
public:
    Device();
    ~Device();
    
    bool initialize();
    void shutdown();
    
    bool is_available() const;
    const char* get_name() const;
    size_t get_memory() const;
    
private:
    class Impl;
    Impl* impl_;
};

} // namespace gpu
} // namespace attic
