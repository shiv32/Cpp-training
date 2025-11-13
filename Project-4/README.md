# std::any Implementation in C++17

A complete implementation of the C++ Standard Library's `std::any` class, which is a type-safe container for single any typed value.

## Overview

`std::any` is a powerful utility in C++ that allows you to store any value type in a single container. This implementation uses **type erasure** to achieve this functionality.

## Features Implemented

### Core Functionality
- ✅ **Type Erasure**: Uses a virtual placeholder pattern to store any type
- ✅ **Copy Semantics**: Copy construction and copy assignment
- ✅ **Move Semantics**: Move construction and move assignment
- ✅ **Type Information**: Runtime type checking via `type()` method
- ✅ **Value Testing**: Check if any contains a value via `has_value()`
- ✅ **any_cast**: Safe casting to retrieve stored values
- ✅ **Exception Safety**: `bad_any_cast` exception for invalid casts
- ✅ **Swap Operations**: Support for swapping any objects
- ✅ **Reset**: Clear the stored value

## How It Works

### Type Erasure Pattern

The implementation uses the **type erasure** design pattern:

```cpp
struct placeholder {           // Base class (virtual)
    virtual ~placeholder() = default;
    virtual std::unique_ptr<placeholder> clone() const = 0;
    virtual const std::type_info& type() const noexcept = 0;
};

template<typename ValueType>
struct holder : public placeholder {  // Derived template class
    ValueType value;
    // Implements virtual methods
};
```

When you store a value in `any`:
1. The value is wrapped in a `holder<T>` object
2. The holder is managed via `std::unique_ptr<placeholder>`
3. At runtime, `type()` returns `typeid(ValueType)`
4. `any_cast` uses type information to safely retrieve the value

### Memory Management

- Uses `std::unique_ptr` for automatic memory management
- No manual `new`/`delete` required
- RAII principles ensure proper cleanup

## API Reference

### Constructors

```cpp
any();                                    // Default constructor (empty)
any(const any& other);                    // Copy constructor
any(any&& other) noexcept;               // Move constructor
template<typename T> any(T&& value);     // Universal constructor
```

### Member Functions

```cpp
any& operator=(const any& rhs);          // Copy assignment
any& operator=(any&& rhs) noexcept;      // Move assignment
template<typename T> any& operator=(T&&);// Universal assignment

void reset() noexcept;                   // Clear the value
void swap(any& other) noexcept;          // Swap contents
bool has_value() const noexcept;         // Check if contains a value
const std::type_info& type() const noexcept;  // Get stored type info
```

### Free Functions

```cpp
template<typename T> T any_cast(const any& operand);
template<typename T> T any_cast(any& operand);
template<typename T> T any_cast(any&& operand);
template<typename T> const T* any_cast(const any* operand) noexcept;
template<typename T> T* any_cast(any* operand) noexcept;
void swap(any& lhs, any& rhs) noexcept;
```

### Exception

```cpp
class bad_any_cast : public std::exception;
```

Thrown when `any_cast` fails with invalid type.

## Usage Examples

### Basic Usage

```cpp
my_std::any a = 42;
std::cout << a.type().name() << "\n";  // Output: int
int val = my_std::any_cast<int>(a);     // val = 42
```

### Type Checking

```cpp
my_std::any a = "hello";
if (a.type() == typeid(const char*)) {
    // Handle const char*
}
```

### Pointer-based Casting (No Exception)

```cpp
my_std::any a = 3.14;
double* ptr = my_std::any_cast<double>(&a);
if (ptr) {
    std::cout << *ptr << "\n";
}
```

### Move Semantics

```cpp
std::vector<int> vec = {1, 2, 3};
my_std::any a = std::move(vec);  // vec is now moved
```

### Custom Types

```cpp
struct Point { int x, y; };
my_std::any a = Point{10, 20};
Point p = my_std::any_cast<Point>(a);
```

### Exception Handling

```cpp
try {
    my_std::any a = 42;
    std::string s = my_std::any_cast<std::string>(a);  // throws
} catch (const my_std::bad_any_cast& e) {
    std::cout << e.what() << "\n";
}
```

## Building and Running

### With CMake

```bash
cd /home/shiv/Projects/Cpp-training/Project-4
mkdir build
cd build
cmake ..
cmake --build .
./any_test
```

### With g++/clang++ Directly

```bash
cd /home/shiv/Projects/Cpp-training/Project-4
g++ -std=c++17 -o any_test main.cpp
./any_test
```

## Implementation Details

### Type Information Storage

The implementation maintains type information at runtime using `std::type_info`. Each holder stores its type:

```cpp
const std::type_info& type() const noexcept override {
    return typeid(ValueType);
}
```

### Copy vs Move

- **Copy**: Clones the entire holder and its contained value
- **Move**: Transfers ownership of the unique_ptr (cheap operation)

### any_cast Behavior

1. **Value-based cast**: Returns a copy or throws `bad_any_cast`
2. **Pointer-based cast**: Returns pointer or `nullptr` (no exception)
3. **Reference cast**: Returns a reference to the stored value

## Key Design Decisions

1. **Type Erasure**: Allows storing any type without knowing it at compile time
2. **unique_ptr**: Provides automatic memory management
3. **Template specialization**: Separate holder<T> for each type
4. **Virtual methods**: Enable runtime type checking and cloning
5. **Namespace**: `my_std` to distinguish from standard library

## Comparison with std::any

This implementation is fully compatible with `std::any` from C++17. It provides:
- Same API surface
- Same behavior and semantics
- Same exception guarantees
- Same move and copy semantics

## Testing

The included `main.cpp` demonstrates:
- Basic construction and type checking (Test 1)
- Storing different types (Test 2)
- any_cast with values (Test 3)
- any_cast with pointers (Test 4)
- Exception handling (Test 5)
- Copy construction (Test 6)
- Move semantics (Test 7)
- Assignment operators (Test 8)
- Reset functionality (Test 9)
- Swap operations (Test 10)
- Custom types (Test 11)
- Empty any edge cases (Test 12)

## Performance Notes

- **Storage**: One additional pointer overhead for the virtual table
- **Copy**: O(n) where n is the size of the stored type (deep copy)
- **Move**: O(1) - just transfers unique_ptr ownership
- **Type check**: O(1) - typeid comparison
- **Access**: O(1) after type verification

## C++17 Features Used

- ✅ `std::unique_ptr` with move semantics
- ✅ `if constexpr` for compile-time type handling
- ✅ `std::enable_if_t` for template constraints
- ✅ `std::is_reference_v` and other type traits
- ✅ `std::remove_cv_t` and `std::remove_reference_t`
- ✅ `std::decay_t` for type decay
- ✅ `std::forward<T>` for perfect forwarding

## License

Educational implementation for learning purposes.
