# Arena Allocator

An arena allocator implementation in C++20, designed as a header-only library.

## Features

- **Header-only**: No linking required.
- **Template-based**: Supports custom alignment values.
- **Exception-safe**: Throws `std::bad_alloc` on allocation failure.
- **Reset support**: Allows resetting the allocator to reuse memory without deallocation.

## Usage Example

```cpp
#include "arena_allocator.hpp"

int main() {
    using namespace arena;
    
    ArenaAllocator<16> alloc(1024); // 1KB buffer with 16-byte alignment
    
    int* ptr = alloc.allocate<int>(5);
    *ptr = 42;
    
    alloc.reset(); // Reset the allocator
    return 0;
}
```

## API Reference

### `ArenaAllocator<Alignment>`

- **`explicit ArenaAllocator(std::size_t capacity)`**: Constructs an arena with a given buffer size.
- **`~ArenaAllocator()`**: Destroys the allocator and frees memory.
- **`allocate<T>(std::size_t n)`**: Allocates space for `n` objects of type `T`.
- **`deallocate<T>(ptr, n)`**: No-op (arena allocators do not support individual deallocation).
- **`reset()`**: Resets the allocator to its initial state.
- **`used_memory() const`**: Returns the amount of memory used in bytes.
- **`capacity() const`**: Returns the total capacity of the buffer in bytes.

### Operators

- `operator==`: Always returns true for equality comparison (all arenas are equal).
- `operator!=`: Always returns false for inequality comparison.

## Compilation

Compile with C++20 support:

```bash
clang++ -std=c++20 main.cpp -o arena_example
```

Or using g++:

```bash
g++ -std=c++20 main.cpp -o arena_example
```