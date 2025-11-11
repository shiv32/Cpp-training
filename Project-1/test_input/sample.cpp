#include <iostream>

class Base {
public:
    virtual ~Base() = default;
};

class Derived : public Base {
public:
    void specificMethod() {}
};

int main() {
    // Testing various C++ casts
    double pi = 3.14159;
    int intPi = static_cast<int>(pi);  // static_cast example
    
    Base* base = new Derived();
    if (Derived* derived = dynamic_cast<Derived*>(base)) {  // dynamic_cast example
        derived->specificMethod();
    }
    
    const int constant = 42;
    const int* constPtr = &constant;
    int* mutable_ptr = const_cast<int*>(constPtr);  // const_cast example
    
    long value = 12345;
    void* raw = reinterpret_cast<void*>(&value);  // reinterpret_cast example
    
    delete base;
    return 0;
}
