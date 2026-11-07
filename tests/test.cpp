#include <iostream>
#include "arena_allocator.hpp"
#include <cassert>
#include <cstddef>

using namespace arena;

void test_basic_allocation() {
    ArenaAllocator<8> alloc(1024);
    int* ptr = alloc.allocate<int>(5);
    assert(ptr != nullptr);
    *ptr = 42;
    assert(*ptr == 42);
}

void test_alignment() {
    ArenaAllocator<16> alloc(1024);
    int* ptr = alloc.allocate<int>(1);
    uintptr_t addr = reinterpret_cast<uintptr_t>(ptr);
    assert((addr & 15) == 0); // Should be aligned to 16 bytes
}

void test_reset() {
    ArenaAllocator<8> alloc(1024);
    int* ptr = alloc.allocate<int>(1);
    assert(alloc.used_memory() > 0);
    alloc.reset();
    assert(alloc.used_memory() == 0);
}

void test_multiple_allocations() {
    ArenaAllocator<8> alloc(256);
    int* p1 = alloc.allocate<int>(3);
    double* p2 = alloc.allocate<double>(2);
    assert(p1 != nullptr && p2 != nullptr);
    for (int i = 0; i < 3; ++i) {
        p1[i] = i;
    }
    for (size_t i = 0; i < 2; ++i) {
        p2[i] = i * 1.5;
    }
}

void test_determinism() {
    ArenaAllocator<8> alloc(1024);
    int* ptr1 = alloc.allocate<int>(1);
    int* ptr2 = alloc.allocate<int>(1);
    assert(ptr1 != nullptr && ptr2 != nullptr);
    // Allocation should be deterministic
    assert(ptr2 > ptr1);
}

void test_equality() {
    ArenaAllocator<8> a(1024), b(1024);
    assert(a == b);
    assert(!(a != b));
}

int main() {
    test_basic_allocation();
    test_alignment();
    test_reset();
    test_multiple_allocations();
    test_determinism();
    test_equality();
    std::cout << "All tests passed!\n";
    return 0;
}