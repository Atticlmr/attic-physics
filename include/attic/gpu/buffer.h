#pragma once
#include <cstddef>

namespace attic {
namespace gpu {

// Structure of Arrays buffer for GPU
struct SoABuffer {
    float* x;
    float* y;
    float* z;
    int count;
    
    void allocate(int n);
    void free();
};

// Body data in SoA format
typedef SoABuffer PositionBuffer;
typedef SoABuffer VelocityBuffer;
typedef SoABuffer ForceBuffer;

} // namespace gpu
} // namespace attic
