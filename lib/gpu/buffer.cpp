#include "attic/gpu/buffer.h"

namespace attic {
namespace gpu {

void SoABuffer::allocate(int n) {
    // TODO: Implement GPU memory allocation for SoA
    count = n;
    x = nullptr;
    y = nullptr;
    z = nullptr;
}

void SoABuffer::free() {
    // TODO: Implement GPU memory deallocation
    count = 0;
    x = nullptr;
    y = nullptr;
    z = nullptr;
}

} // namespace gpu
} // namespace attic
