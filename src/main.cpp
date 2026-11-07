#include "arena_allocator.hpp"
#include <iostream>

int main() {
    using namespace arena;

    try {
        ArenaAllocator<16> allocator(1024);

        int* p1 = allocator.allocate<int>(5);
        double* p2 = allocator.allocate<double>(3);

        for (int i = 0; i < 5; ++i) {
            p1[i] = i * 2;
        }

        for (int i = 0; i < 3; ++i) {
            p2[i] = i * 1.5;
        }

        std::cout << "Allocated memory: " << allocator.used_memory() << " bytes\n";
        std::cout << "Capacity: " << allocator.capacity() << " bytes\n";

        // Reset the arena
        allocator.reset();
        std::cout << "After reset - Used memory: " << allocator.used_memory() << " bytes\n";

    } catch (const std::bad_alloc& e) {
        std::cerr << "Allocation failed: " << e.what() << '\n';
        return 1;
    }

    return 0;
}