#pragma once

#include <cstddef>
#include <cstdint>
#include <cstdlib>

class AlignedBuffer {
public:
    AlignedBuffer() : data_(nullptr), size_(0) {}
    explicit AlignedBuffer(size_t size, size_t alignment = 64) { allocate(size, alignment); }
    ~AlignedBuffer() { free(); }

    void allocate(size_t size, size_t alignment = 64) {
        free();
#if defined(_ISOC11_SOURCE) || (__STDC_VERSION__ >= 201112L)
        data_ = aligned_alloc(alignment, ((size + alignment - 1) / alignment) * alignment);
        if (!data_) data_ = nullptr;
#else
        if (posix_memalign(&data_, alignment, size) != 0) data_ = nullptr;
#endif
        size_ = data_ ? size : 0;
    }

    void free() {
        if (data_) {
            std::free(data_);
            data_ = nullptr;
            size_ = 0;
        }
    }

    void *data() { return data_; }
    const void *data() const { return data_; }
    size_t size() const { return size_; }

private:
    void *data_;
    size_t size_;
};
