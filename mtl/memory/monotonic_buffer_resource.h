#pragma once

#include "memory_resource.h"
#include <cassert>

namespace mtl {
class monotonic_buffer_resource
    : public memory_resource
{
public:
    monotonic_buffer_resource(void* buffer, size_t bytes)
        : buffer_(buffer)
        , space_available_(bytes)
    {
        assert(space_available_);
        assert(buffer_);
    }

protected:
    virtual void* do_allocate(const size_t bytes, const size_t align) override
    {
        LOG_PROCESS_ERROR(space_available_ >= bytes);
        void* const result = buffer_;
        buffer_ = reinterpret_cast<char*>(buffer_) + bytes;
        space_available_ -= bytes;
        return result;
    }

	virtual void do_deallocate(void*, const size_t, const size_t) override
    {
        // nothing to do
    }

private:
    void* buffer_;
    size_t space_available_;
};
} // namespace mtl