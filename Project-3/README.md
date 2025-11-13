# C++17 std::optional Implementation

A complete implementation of the `std::optional` class from C++17 standard library, created from scratch without using the standard library's version.

## Features

This implementation provides a full-featured `optional<T>` class with:

### Core Functionality
- **Default construction**: Creates an empty optional
- **Value construction**: Direct initialization with a value
- **nullopt support**: Initialize with `nullopt` to explicitly create empty optional
- **Copy and move semantics**: Full copy and move construction/assignment
- **In-place construction**: Using `in_place` for efficient initialization

### Value Access
- **Operator\***: Dereference to access the contained value
- **Operator->**: Member access through the optional
- **value()**: Retrieve value with bounds checking (throws `bad_optional_access` if empty)
- **value_or()**: Retrieve value with a default fallback
- **has_value()**: Check if optional contains a value
- **Explicit bool conversion**: Use in boolean context (if statements, etc.)

### Modification
- **Assignment operators**: Support assignment from values, other optionals, and `nullopt`
- **emplace()**: Construct value in-place, replacing any existing value
- **reset()**: Clear the optional, destroying the contained value
- **swap()**: Exchange contents with another optional

### Comparison
- **Equality/Inequality**: Compare with other optionals, values, and nullopt
- **Relational operators**: `<`, `<=`, `>`, `>=` for ordering

### Exception Safety
- **bad_optional_access**: Exception thrown when accessing empty optional via `value()`
- **Proper cleanup**: Destructors called for stored values

## Building with CMake

```bash
# Configure the project
cmake -B build -S .

# Build the project
cmake --build build

# Run tests
./build/test_optional
```

## Building with Make (Alternative)

```bash
# Build and run
make
make run

# Clean build artifacts
make clean
```

## Project Structure

- **optional.hpp**: Complete implementation of `optional<T>` template class
- **test_optional.cpp**: Comprehensive test suite demonstrating all features
- **CMakeLists.txt**: CMake build configuration
- **Makefile**: Alternative Make build configuration

## Implementation Details

### Storage
The implementation uses a union-based storage approach for memory efficiency:
- A union containing either a dummy character or the actual value
- Manual construction/destruction using placement new and explicit destructors

### Template Specialization
- Main template for arbitrary types `T`
- Specialization for `void` type with empty storage

### SFINAE and Type Traits
- Careful use of `std::enable_if_t` to prevent unwanted conversions
- Type traits like `std::is_nothrow_constructible_v` for optimization
- Proper noexcept specifications based on type properties

### Value Categories
- Different overloads for `operator*` and `value()` handling lvalue/rvalue references
- Proper forwarding of move semantics throughout

## Test Coverage

The test suite includes 24 comprehensive tests covering:
1. Default construction
2. Value construction
3. nullopt construction
4. Copy/move operations
5. Dereference operators
6. Member access
7. Bool conversion
8. Value access and exceptions
9. Default values
10. Assignment operations
11. reset() functionality
12. In-place construction and emplacement
13. Comparison operators
14. Nullopt comparisons
15. Value comparisons
16. Swapping
17. Complex types (strings)
18. Type conversions

## Standards Compliance

- **C++17**: Uses C++17 features like structured bindings, constexpr, and template features
- **Standard Library Compatibility**: Mirrors the interface of `std::optional<T>`
- **Exception Safety**: Strong exception guarantees where applicable

## Example Usage

```cpp
#include "optional.hpp"

optional<int> divide(int a, int b) {
    if (b == 0) {
        return nullopt;  // Return empty optional
    }
    return a / b;        // Return value
}

int main() {
    auto result = divide(10, 2);
    
    if (result) {
        std::cout << "Result: " << *result << std::endl;
    }
    
    // Or with value_or
    int value = divide(10, 0).value_or(-1);
    std::cout << "Value: " << value << std::endl;
    
    return 0;
}
```

## Compiler Requirements

- GCC 7+ or Clang 5+ with C++17 support
- Tested with GCC 15.2.1
