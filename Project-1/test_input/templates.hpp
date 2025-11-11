template<typename T>
class SmartPtr {
    T* ptr;
public:
    explicit SmartPtr(T* p) : ptr(p) {}
    
    template<typename U>
    SmartPtr<U>* dynamicCastTo() {
        U* castedPtr = dynamic_cast<U*>(ptr);
        return castedPtr ? new SmartPtr<U>(castedPtr) : nullptr;
    }
};
