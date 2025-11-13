#include "any.hpp"
#include <iostream>
#include <string>
#include <vector>

void print_separator(const std::string& title) {
    std::cout << "\n" << std::string(50, '=') << "\n";
    std::cout << "  " << title << "\n";
    std::cout << std::string(50, '=') << "\n";
}

int main() {
    using namespace my_std;

    // Test 1: Basic construction and type checking
    print_separator("Test 1: Basic Construction");
    
    any a;
    std::cout << "Empty any - has_value: " << std::boolalpha << a.has_value() << "\n";
    std::cout << "Empty any - type: " << a.type().name() << "\n";
    
    any a1 = 42;
    std::cout << "any(42) - has_value: " << a1.has_value() << "\n";
    std::cout << "any(42) - type: " << a1.type().name() << "\n";

    // Test 2: Storing different types
    print_separator("Test 2: Storing Different Types");
    
    any a_int = 100;
    any a_double = 3.14;
    any a_string = std::string("Hello, any!");
    any a_vector = std::vector<int>{1, 2, 3, 4, 5};
    
    std::cout << "int: " << a_int.type().name() << "\n";
    std::cout << "double: " << a_double.type().name() << "\n";
    std::cout << "string: " << a_string.type().name() << "\n";
    std::cout << "vector: " << a_vector.type().name() << "\n";

    // Test 3: any_cast with values
    print_separator("Test 3: any_cast with Values");
    
    try {
        int val = any_cast<int>(a_int);
        std::cout << "Cast int: " << val << "\n";
        
        double val2 = any_cast<double>(a_double);
        std::cout << "Cast double: " << val2 << "\n";
        
        const std::string& str = any_cast<const std::string&>(a_string);
        std::cout << "Cast string: " << str << "\n";
    } catch (const bad_any_cast& e) {
        std::cout << "Error: " << e.what() << "\n";
    }

    // Test 4: any_cast with pointers
    print_separator("Test 4: any_cast with Pointers");
    
    int* ptr = any_cast<int>(&a_int);
    if (ptr) {
        std::cout << "Pointer cast successful: " << *ptr << "\n";
    }
    
    // Wrong type cast with pointer (should return nullptr)
    double* ptr_fail = any_cast<double>(&a_int);
    std::cout << "Wrong type cast (pointer): " << (ptr_fail ? "Success" : "nullptr (expected)") << "\n";

    // Test 5: Exception handling for bad_any_cast
    print_separator("Test 5: Exception Handling");
    
    try {
        any_cast<int>(a_string);
        std::cout << "This should not print\n";
    } catch (const bad_any_cast& e) {
        std::cout << "Caught exception: " << e.what() << "\n";
    }

    // Test 6: Copy construction
    print_separator("Test 6: Copy Construction");
    
    any b = a_string;
    std::cout << "Original type: " << a_string.type().name() << "\n";
    std::cout << "Copy type: " << b.type().name() << "\n";
    
    std::string copied_str = any_cast<std::string>(b);
    std::cout << "Copied value: " << copied_str << "\n";

    // Test 7: Move semantics
    print_separator("Test 7: Move Semantics");
    
    any c = std::move(a_vector);
    std::cout << "After move, source has_value: " << a_vector.has_value() << "\n";
    std::cout << "Moved to destination has_value: " << c.has_value() << "\n";
    
    std::vector<int> moved_vec = any_cast<std::vector<int>>(c);
    std::cout << "Moved vector: ";
    for (int x : moved_vec) {
        std::cout << x << " ";
    }
    std::cout << "\n";

    // Test 8: Assignment operators
    print_separator("Test 8: Assignment Operators");
    
    any d;
    d = 123;
    std::cout << "After d = 123: " << any_cast<int>(d) << "\n";
    
    d = std::string("reassigned");
    std::cout << "After d = string: " << any_cast<std::string>(d) << "\n";
    
    d = a_int;
    std::cout << "After d = any(int): " << any_cast<int>(d) << "\n";

    // Test 9: Reset
    print_separator("Test 9: Reset");
    
    any e = 999;
    std::cout << "Before reset - has_value: " << e.has_value() << "\n";
    e.reset();
    std::cout << "After reset - has_value: " << e.has_value() << "\n";
    std::cout << "After reset - type: " << e.type().name() << "\n";

    // Test 10: Swap
    print_separator("Test 10: Swap");
    
    any f = 111;
    any g = std::string("swap me");
    
    std::cout << "Before swap:\n";
    std::cout << "  f: " << any_cast<int>(f) << "\n";
    std::cout << "  g: " << any_cast<std::string>(g) << "\n";
    
    f.swap(g);
    
    std::cout << "After swap:\n";
    std::cout << "  f: " << any_cast<std::string>(f) << "\n";
    std::cout << "  g: " << any_cast<int>(g) << "\n";

    // Test 11: Custom types
    print_separator("Test 11: Custom Types");
    
    struct Point {
        int x, y;
        Point(int x = 0, int y = 0) : x(x), y(y) {}
    };
    
    any point_any = Point(10, 20);
    Point p = any_cast<Point>(point_any);
    std::cout << "Custom type Point(" << p.x << ", " << p.y << ")\n";

    // Test 12: Empty any edge cases
    print_separator("Test 12: Empty any Edge Cases");
    
    any empty;
    std::cout << "Empty any type check: " << empty.type().name() << "\n";
    
    int* empty_ptr = any_cast<int>(&empty);
    std::cout << "any_cast pointer on empty: " << (empty_ptr ? "not null" : "nullptr (expected)") << "\n";

    print_separator("All Tests Completed");
    return 0;
}
