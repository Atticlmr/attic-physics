#include "attic/gpu/device.h"

namespace attic {
namespace gpu {

class Device::Impl {
public:
    bool initialized = false;
};

Device::Device() : impl_(new Impl()) {}

Device::~Device() {
    delete impl_;
}

bool Device::initialize() {
    // TODO: Implement CUDA/Vulkan initialization
    impl_->initialized = true;
    return true;
}

void Device::shutdown() {
    impl_->initialized = false;
}

bool Device::is_available() const {
    return impl_->initialized;
}

const char* Device::get_name() const {
    return "GPU Device";
}

size_t Device::get_memory() const {
    return 0;
}

} // namespace gpu
} // namespace attic
