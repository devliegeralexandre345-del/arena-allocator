#pragma once
#include <cstddef>
#include <cstdint>
#include <new>
#include <type_traits>
#include <cstdlib>

namespace arena {

    template<std::size_t Alignment = alignof(std::max_align_t)>
    class ArenaAllocator {
        static_assert(Alignment > 0 && (Alignment & (Alignment - 1)) == 0, "Alignment must be a power of two");

    private:
        std::byte* buffer_;
        std::size_t capacity_;
        std::size_t offset_;

        static constexpr std::size_t align_up(std::size_t n) {
            return (n + Alignment - 1) & ~(Alignment - 1);
        }

    public:
        explicit ArenaAllocator(std::size_t capacity)
            : buffer_(static_cast<std::byte*>(std::malloc(capacity))),
              capacity_(capacity),
              offset_(0) {
            if (!buffer_) {
                throw std::bad_alloc();
            }
        }

        ~ArenaAllocator() {
            std::free(buffer_);
        }

        ArenaAllocator(const ArenaAllocator&) = delete;
        ArenaAllocator& operator=(const ArenaAllocator&) = delete;

        ArenaAllocator(ArenaAllocator&& other) noexcept
            : buffer_(other.buffer_),
              capacity_(other.capacity_),
              offset_(other.offset_) {
            other.buffer_ = nullptr;
            other.capacity_ = 0;
            other.offset_ = 0;
        }

        ArenaAllocator& operator=(ArenaAllocator&& other) noexcept {
            if (this != &other) {
                std::free(buffer_);
                buffer_ = other.buffer_;
                capacity_ = other.capacity_;
                offset_ = other.offset_;
                other.buffer_ = nullptr;
                other.capacity_ = 0;
                other.offset_ = 0;
            }
            return *this;
        }

        template<typename T>
        T* allocate(std::size_t n) {
            std::size_t aligned_offset = align_up(offset_);
            if (aligned_offset + sizeof(T) * n > capacity_) {
                throw std::bad_alloc();
            }
            T* ptr = reinterpret_cast<T*>(buffer_ + aligned_offset);
            offset_ = aligned_offset + sizeof(T) * n;
            return ptr;
        }

        template<typename T>
        void deallocate(T*, std::size_t) noexcept {
            // Arena allocator does not support individual deallocation.
            // Memory is freed only when the arena itself is destroyed.
        }

        inline void reset() {
            offset_ = 0;
        }

        inline std::size_t used_memory() const {
            return offset_;
        }

        inline std::size_t capacity() const {
            return capacity_;
        }
    };

    template<std::size_t Alignment>
    bool operator==(const ArenaAllocator<Alignment>&, const ArenaAllocator<Alignment>&) {
        return true; // All arena allocators are equal for comparison purposes.
    }

    template<std::size_t Alignment>
    bool operator!=(const ArenaAllocator<Alignment>&, const ArenaAllocator<Alignment>&) {
        return false;
    }

} // namespace arena