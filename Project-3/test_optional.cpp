#include "optional.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <cassert>

void print_separator(const std::string& title) {
    std::cout << "\n=== " << title << " ===\n";
}

int main() {
    // Test 1: Default construction
    print_separator("Test 1: Default Construction");
    optional<int> opt1;
    assert(!opt1.has_value());
    std::cout << "✓ Default constructed optional is empty\n";

    // Test 2: Construction with value
    print_separator("Test 2: Construction with Value");
    optional<int> opt2(42);
    assert(opt2.has_value());
    assert(*opt2 == 42);
    std::cout << "✓ Optional constructed with value 42\n";
    std::cout << "✓ Value: " << *opt2 << "\n";

    // Test 3: Construction with nullopt
    print_separator("Test 3: Construction with nullopt");
    optional<int> opt3(nullopt);
    assert(!opt3.has_value());
    std::cout << "✓ Optional constructed with nullopt is empty\n";

    // Test 4: Copy constructor
    print_separator("Test 4: Copy Constructor");
    optional<int> opt4 = opt2;
    assert(opt4.has_value());
    assert(*opt4 == 42);
    std::cout << "✓ Copy constructor works correctly\n";

    // Test 5: Move constructor
    print_separator("Test 5: Move Constructor");
    optional<std::string> opt5("Hello");
    optional<std::string> opt6 = std::move(opt5);
    assert(opt6.has_value());
    assert(*opt6 == "Hello");
    std::cout << "✓ Move constructor works correctly\n";
    std::cout << "✓ Value: " << *opt6 << "\n";

    // Test 6: Operator*
    print_separator("Test 6: Operator* (Dereference)");
    optional<int> opt7(100);
    int value = *opt7;
    assert(value == 100);
    std::cout << "✓ Operator* works correctly\n";
    std::cout << "✓ Value: " << value << "\n";

    // Test 7: Operator->
    print_separator("Test 7: Operator-> (Member Access)");
    struct Point {
        int x, y;
        Point() : x(0), y(0) {}
        Point(int x_, int y_) : x(x_), y(y_) {}
    };
    optional<Point> opt8(Point{5, 10});
    assert(opt8->x == 5);
    assert(opt8->y == 10);
    std::cout << "✓ Operator-> works correctly\n";
    std::cout << "✓ Point: (" << opt8->x << ", " << opt8->y << ")\n";

    // Test 8: Explicit bool conversion
    print_separator("Test 8: Explicit bool Conversion");
    optional<int> opt9(50);
    if (opt9) {
        std::cout << "✓ Optional is truthy when it has a value\n";
    }
    optional<int> opt10;
    if (!opt10) {
        std::cout << "✓ Optional is falsy when it's empty\n";
    }

    // Test 9: value() method
    print_separator("Test 9: value() Method");
    optional<int> opt11(99);
    assert(opt11.value() == 99);
    std::cout << "✓ value() returns the stored value\n";
    std::cout << "✓ Value: " << opt11.value() << "\n";

    // Test 10: value() throws on empty optional
    print_separator("Test 10: value() Throws on Empty");
    optional<int> opt12;
    try {
        opt12.value();
        assert(false); // Should not reach here
    } catch (const bad_optional_access& e) {
        std::cout << "✓ Caught bad_optional_access: " << e.what() << "\n";
    }

    // Test 11: value_or()
    print_separator("Test 11: value_or()");
    optional<int> opt13(77);
    assert(opt13.value_or(0) == 77);
    optional<int> opt14;
    assert(opt14.value_or(42) == 42);
    std::cout << "✓ value_or() returns value when present\n";
    std::cout << "✓ Value: " << opt13.value_or(0) << "\n";
    std::cout << "✓ value_or() returns default when absent\n";
    std::cout << "✓ Default: " << opt14.value_or(42) << "\n";

    // Test 12: Copy assignment
    print_separator("Test 12: Copy Assignment");
    optional<int> opt15;
    optional<int> opt16(88);
    opt15 = opt16;
    assert(opt15.has_value());
    assert(*opt15 == 88);
    std::cout << "✓ Copy assignment works correctly\n";

    // Test 13: Move assignment
    print_separator("Test 13: Move Assignment");
    optional<std::string> opt17;
    optional<std::string> opt18("World");
    opt17 = std::move(opt18);
    assert(opt17.has_value());
    assert(*opt17 == "World");
    std::cout << "✓ Move assignment works correctly\n";

    // Test 14: Assignment from value
    print_separator("Test 14: Assignment from Value");
    optional<int> opt19;
    opt19 = 200;
    assert(opt19.has_value());
    assert(*opt19 == 200);
    std::cout << "✓ Assignment from value works correctly\n";

    // Test 15: Assignment from nullopt
    print_separator("Test 15: Assignment from nullopt");
    optional<int> opt20(300);
    opt20 = nullopt;
    assert(!opt20.has_value());
    std::cout << "✓ Assignment from nullopt clears the optional\n";

    // Test 16: reset()
    print_separator("Test 16: reset()");
    optional<int> opt21(400);
    opt21.reset();
    assert(!opt21.has_value());
    std::cout << "✓ reset() clears the optional\n";

    // Test 17: emplace()
    print_separator("Test 17: emplace()");
    optional<std::string> opt22;
    opt22.emplace("Emplaced");
    assert(opt22.has_value());
    assert(*opt22 == "Emplaced");
    std::cout << "✓ emplace() constructs value in-place\n";
    std::cout << "✓ Value: " << *opt22 << "\n";

    // Test 18: in_place construction
    print_separator("Test 18: In-place Construction");
    optional<Point> opt23(in_place, 15, 25);
    assert(opt23.has_value());
    assert(opt23->x == 15 && opt23->y == 25);
    std::cout << "✓ In-place construction works\n";
    std::cout << "✓ Point: (" << opt23->x << ", " << opt23->y << ")\n";

    // Test 19: Comparison with optional
    print_separator("Test 19: Comparison Operators");
    optional<int> opt24(42);
    optional<int> opt25(42);
    optional<int> opt26(50);
    optional<int> opt27;
    assert(opt24 == opt25);
    assert(opt24 != opt26);
    assert(opt24 < opt26);
    assert(opt26 > opt24);
    assert(opt27 != opt24);
    std::cout << "✓ Comparison operators work correctly\n";

    // Test 20: Comparison with nullopt
    print_separator("Test 20: Comparison with nullopt");
    optional<int> opt28(50);
    optional<int> opt29;
    assert(opt29 == nullopt);
    assert(opt28 != nullopt);
    assert(nullopt == opt29);
    std::cout << "✓ Comparison with nullopt works\n";

    // Test 21: Comparison with value
    print_separator("Test 21: Comparison with Value");
    optional<int> opt30(42);
    assert(opt30 == 42);
    assert(42 == opt30);
    assert(opt30 != 50);
    assert(50 != opt30);
    std::cout << "✓ Comparison with value works\n";

    // Test 22: swap()
    print_separator("Test 22: swap()");
    optional<int> opt31(100);
    optional<int> opt32(200);
    opt31.swap(opt32);
    assert(*opt31 == 200);
    assert(*opt32 == 100);
    std::cout << "✓ swap() works correctly\n";
    std::cout << "✓ After swap: opt31 = " << *opt31 << ", opt32 = " << *opt32 << "\n";

    // Test 23: Complex type
    print_separator("Test 23: Complex Type (std::string)");
    optional<std::string> opt33("Complex");
    assert(opt33.has_value());
    assert(opt33->length() == 7);
    std::cout << "✓ Works with std::string\n";
    std::cout << "✓ String: " << *opt33 << " (length: " << opt33->length() << ")\n";

    // Test 24: Type conversion
    print_separator("Test 24: Type Conversion");
    optional<int> opt34(10);
    optional<double> opt35(opt34.value()); // Implicit conversion
    assert(*opt35 == 10.0);
    std::cout << "✓ Type conversion works\n";
    std::cout << "✓ int: " << opt34.value() << ", double: " << opt35.value() << "\n";

    print_separator("All Tests Passed!");
    std::cout << "\n✓ The optional<T> implementation is complete and working correctly!\n\n";

    return 0;
}
