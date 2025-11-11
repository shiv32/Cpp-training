# C++17 string_view Implementation

A minimal, header-only implementation of `std::string_view` for educational purposes, demonstrating key concepts of the C++17 standard library class.

## Overview

This project implements a custom `string_view` class in the `project2` namespace that provides a non-owning view of a string. It avoids the need to copy or allocate memory when working with string data.

## Features

- **Non-owning view**: Points to existing string data without copying
- **Lightweight**: Header-only, no external dependencies
- **C++17 compatible**: Uses standard C++17 features like `constexpr`
- **Rich API**: Includes most common `string_view` operations

## API Reference

### Constructors

```cpp
string_view()                              // Empty view
string_view(const char* s)                 // From C-string
string_view(const char* s, size_t count)   // From pointer and length
string_view(const std::string& s)          // From std::string
```

### Observers

```cpp
const char* data() const           // Pointer to data
size_t size() const                // Number of characters
size_t length() const              // Same as size()
bool empty() const                 // Check if empty
const char& operator[](size_t pos) const
const char& at(size_t pos) const   // With bounds checking
const char& front() const          // First character
const char& back() const           // Last character
```

### Modifiers

```cpp
void remove_prefix(size_t n)  // Remove n characters from front
void remove_suffix(size_t n)  // Remove n characters from back
void swap(string_view& other) // Swap with another view
```

### Iterators

```cpp
const_iterator begin() const
const_iterator end() const
const_iterator cbegin() const
const_iterator cend() const
```

### String Operations

```cpp
string_view substr(size_t pos = 0, size_t count = npos) const
int compare(string_view other) const
size_t find(char c, size_t pos = 0) const
size_t find(const char* s, size_t pos = 0) const
size_t find(string_view sv, size_t pos = 0) const
size_t rfind(char c, size_t pos = npos) const
size_t rfind(string_view sv, size_t pos = npos) const
bool starts_with(string_view sv) const
bool ends_with(string_view sv) const
std::string to_string() const
```

### Relational Operators

```cpp
bool operator==(const string_view& a, const string_view& b)
bool operator!=(const string_view& a, const string_view& b)
bool operator<(const string_view& a, const string_view& b)
bool operator<=(const string_view& a, const string_view& b)
bool operator>(const string_view& a, const string_view& b)
bool operator>=(const string_view& a, const string_view& b)
```

### Stream Output

```cpp
std::ostream& operator<<(std::ostream& os, const string_view& sv)
```

## Building

### With CMake

```bash
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

### Manually

```bash
g++ -std=c++17 main.cpp -o main
```

## Running

```bash
./main
```

### Expected Output

```
sv1: Hello, world! (size=13)
sv2: C++ string
"world" at 7
part: world
starts_with "Hello": true
ends_with "world!": true
sv2 after remove_prefix(4): string
sv2 after remove_suffix(3): str
compare sv1 and "Hello": 1
to_string (via ctor): Hello, world!
characters in sv1: H e l l o ,   w o r l d !
```

## Project Structure

```
Project-2/
├── CMakeLists.txt           # CMake build configuration
├── README.md                # This file
├── main.cpp                 # Example usage
└── include/
    └── string_view.hpp      # Implementation
```

## Key Implementation Details

- **No allocation**: The implementation only stores a pointer and size
- **Constexpr support**: Many functions are `constexpr` for compile-time evaluation
- **Iterator support**: Full range-based for loop support
- **Error handling**: `at()` and `substr()` throw `std::out_of_range` for invalid positions
- **npos constant**: Special value indicating "not found" (static_cast<size_t>(-1))

## Differences from std::string_view

This implementation is a **subset** of the standard library's `std::string_view`:

- Includes most common operations
- May not handle all edge cases identically
- Designed for educational purposes, not production use
- Placed in `project2` namespace to avoid conflicts

## Use Cases

- **Zero-copy substring operations**: Create views without allocating memory
- **Function parameters**: Pass string references efficiently
- **String parsing**: Work with portions of strings safely
- **Performance**: Avoid unnecessary string copies in performance-critical code

## License

Educational implementation for learning purposes.
