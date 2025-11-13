#ifndef MY_ANY_HPP
#define MY_ANY_HPP

#include <typeinfo>
#include <memory>
#include <utility>
#include <type_traits>
#include <stdexcept>

namespace my_std {

class bad_any_cast : public std::exception {
public:
    const char* what() const noexcept override {
        return "bad any_cast";
    }
};

class any {
private:
    // Base class for type erasure
    struct placeholder {
        virtual ~placeholder() = default;
        virtual std::unique_ptr<placeholder> clone() const = 0;
        virtual const std::type_info& type() const noexcept = 0;
    };

    // Derived class template for holding actual values
    template<typename ValueType>
    struct holder : public placeholder {
        ValueType value;

        template<typename T>
        explicit holder(T&& val) 
            : value(std::forward<T>(val)) {}

        std::unique_ptr<placeholder> clone() const override {
            return std::make_unique<holder<ValueType>>(value);
        }

        const std::type_info& type() const noexcept override {
            return typeid(ValueType);
        }
    };

    std::unique_ptr<placeholder> content;

public:
    // Constructors
    constexpr any() noexcept = default;

    any(const any& other) {
        if (other.content) {
            content = other.content->clone();
        }
    }

    any(any&& other) noexcept = default;

    template<typename ValueType,
             typename = std::enable_if_t<!std::is_same_v<std::decay_t<ValueType>, any>>>
    any(ValueType&& value) 
        : content(std::make_unique<holder<std::decay_t<ValueType>>>(
            std::forward<ValueType>(value))) {}

    // Destructor
    ~any() = default;

    // Assignment operators
    any& operator=(const any& rhs) {
        any(rhs).swap(*this);
        return *this;
    }

    any& operator=(any&& rhs) noexcept {
        any(std::move(rhs)).swap(*this);
        return *this;
    }

    template<typename ValueType,
             typename = std::enable_if_t<!std::is_same_v<std::decay_t<ValueType>, any>>>
    any& operator=(ValueType&& rhs) {
        any(std::forward<ValueType>(rhs)).swap(*this);
        return *this;
    }

    // Modifiers
    void reset() noexcept {
        content.reset();
    }

    void swap(any& other) noexcept {
        content.swap(other.content);
    }

    // Observers
    bool has_value() const noexcept {
        return content != nullptr;
    }

    const std::type_info& type() const noexcept {
        if (!content) {
            return typeid(void);
        }
        return content->type();
    }

    // Friend function for any_cast
    template<typename T>
    friend T any_cast(const any& operand);

    template<typename T>
    friend T any_cast(any& operand);

    template<typename T>
    friend T any_cast(any&& operand);

    template<typename T>
    friend const T* any_cast(const any* operand) noexcept;

    template<typename T>
    friend T* any_cast(any* operand) noexcept;
};

// any_cast implementations
template<typename T>
T any_cast(const any& operand) {
    using value_type = std::remove_cv_t<std::remove_reference_t<T>>;
    
    const auto* result = any_cast<value_type>(&operand);
    if (!result) {
        throw bad_any_cast();
    }
    
    if constexpr (std::is_reference_v<T>) {
        return *const_cast<value_type*>(result);
    } else {
        return *result;
    }
}

template<typename T>
T any_cast(any& operand) {
    using value_type = std::remove_cv_t<std::remove_reference_t<T>>;
    
    auto* result = any_cast<value_type>(&operand);
    if (!result) {
        throw bad_any_cast();
    }
    
    if constexpr (std::is_reference_v<T>) {
        return *result;
    } else {
        return *result;
    }
}

template<typename T>
T any_cast(any&& operand) {
    using value_type = std::remove_cv_t<std::remove_reference_t<T>>;
    
    auto* result = any_cast<value_type>(&operand);
    if (!result) {
        throw bad_any_cast();
    }
    
    if constexpr (std::is_reference_v<T>) {
        return std::forward<T>(*result);
    } else {
        return std::move(*result);
    }
}

template<typename T>
const T* any_cast(const any* operand) noexcept {
    if (!operand || operand->type() != typeid(T)) {
        return nullptr;
    }
    
    const auto* holder = static_cast<const any::holder<T>*>(operand->content.get());
    return &holder->value;
}

template<typename T>
T* any_cast(any* operand) noexcept {
    if (!operand || operand->type() != typeid(T)) {
        return nullptr;
    }
    
    auto* holder = static_cast<any::holder<T>*>(operand->content.get());
    return &holder->value;
}

// Non-member swap
inline void swap(any& lhs, any& rhs) noexcept {
    lhs.swap(rhs);
}

} // namespace my_std

#endif // MY_ANY_HPP
