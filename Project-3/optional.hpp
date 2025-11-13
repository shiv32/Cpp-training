#ifndef OPTIONAL_HPP
#define OPTIONAL_HPP

#include <stdexcept>
#include <utility>
#include <type_traits>
#include <new>

// Nullopt type to represent an empty optional
struct nullopt_t {};
constexpr nullopt_t nullopt{};

// Bad optional access exception
class bad_optional_access : public std::logic_error {
public:
    bad_optional_access() : std::logic_error("Bad optional access") {}
};

// Forward declaration
template <typename T>
class optional;

// in_place_t for in-place construction
struct in_place_t {};
constexpr in_place_t in_place{};

// Storage for optional
template <typename T>
union optional_storage {
    char dummy;
    T value;

    constexpr optional_storage() : dummy('\0') {}
    constexpr optional_storage(const T& v) : value(v) {}
    constexpr optional_storage(T&& v) : value(std::move(v)) {}

    ~optional_storage() {}
};

// Template specialization for void (empty storage)
template <>
union optional_storage<void> {
    char dummy;

    constexpr optional_storage() : dummy('\0') {}
    ~optional_storage() {}
};

// Main optional class
template <typename T>
class optional {
public:
    using value_type = T;

    // Default constructor - creates an empty optional
    constexpr optional() noexcept : storage_(), has_value_(false) {}

    // Destructor
    ~optional() noexcept {
        if (has_value_) {
            storage_.value.~T();
        }
    }

    // Constructor from nullopt
    constexpr optional(nullopt_t) noexcept : storage_(), has_value_(false) {}

    // Copy constructor
    optional(const optional& other) noexcept(std::is_nothrow_copy_constructible_v<T>)
        : has_value_(other.has_value_) {
        if (other.has_value_) {
            new (&storage_.value) T(other.storage_.value);
        }
    }

    // Move constructor
    optional(optional&& other) noexcept(std::is_nothrow_move_constructible_v<T>)
        : has_value_(other.has_value_) {
        if (other.has_value_) {
            new (&storage_.value) T(std::move(other.storage_.value));
        }
    }

    // Constructor from value
    template <typename U = T,
              typename = std::enable_if_t<!std::is_same_v<std::decay_t<U>, optional>>>
    constexpr optional(U&& value) noexcept(std::is_nothrow_constructible_v<T, U>)
        : storage_(std::forward<U>(value)), has_value_(true) {}

    // Constructor from optional<U>
    template <typename U,
              typename = std::enable_if_t<std::is_constructible_v<T, const U&> &&
                                          !std::is_constructible_v<T, optional<U>&> &&
                                          !std::is_constructible_v<T, optional<U>&&> &&
                                          !std::is_convertible_v<optional<U>&, T> &&
                                          !std::is_convertible_v<optional<U>&&, T>>>
    optional(const optional<U>& other) noexcept(std::is_nothrow_constructible_v<T, const U&>) {
        if (other) {
            storage_.value = T(*other);
            has_value_ = true;
        }
    }

    // In-place constructor
    template <typename... Args>
    constexpr explicit optional(in_place_t, Args&&... args) noexcept(
        std::is_nothrow_constructible_v<T, Args...>)
        : has_value_(true) {
        new (&storage_.value) T(std::forward<Args>(args)...);
    }

    // Copy assignment
    optional& operator=(const optional& other) noexcept(
        std::is_nothrow_copy_constructible_v<T> && std::is_nothrow_copy_assignable_v<T>) {
        if (this != &other) {
            if (has_value_ && other.has_value_) {
                storage_.value = other.storage_.value;
            } else if (other.has_value_) {
                new (&storage_.value) T(other.storage_.value);
                has_value_ = true;
            } else {
                reset();
            }
        }
        return *this;
    }

    // Move assignment
    optional& operator=(optional&& other) noexcept(
        std::is_nothrow_move_constructible_v<T> && std::is_nothrow_move_assignable_v<T>) {
        if (this != &other) {
            if (has_value_ && other.has_value_) {
                storage_.value = std::move(other.storage_.value);
            } else if (other.has_value_) {
                new (&storage_.value) T(std::move(other.storage_.value));
                has_value_ = true;
            } else {
                reset();
            }
        }
        return *this;
    }

    // Assignment from nullopt
    optional& operator=(nullopt_t) noexcept {
        reset();
        return *this;
    }

    // Assignment from value
    template <typename U = T,
              typename = std::enable_if_t<!std::is_same_v<std::decay_t<U>, optional>>>
    optional& operator=(U&& value) noexcept(std::is_nothrow_assignable_v<T&, U>) {
        if (has_value_) {
            storage_.value = std::forward<U>(value);
        } else {
            new (&storage_.value) T(std::forward<U>(value));
            has_value_ = true;
        }
        return *this;
    }

    // Observers
    constexpr const T* operator->() const noexcept { return &storage_.value; }

    constexpr T* operator->() noexcept { return &storage_.value; }

    constexpr const T& operator*() const& noexcept { return storage_.value; }

    constexpr T& operator*() & noexcept { return storage_.value; }

    constexpr const T&& operator*() const&& noexcept { return std::move(storage_.value); }

    constexpr T&& operator*() && noexcept { return std::move(storage_.value); }

    // Conversion to bool
    constexpr explicit operator bool() const noexcept { return has_value_; }

    constexpr bool has_value() const noexcept { return has_value_; }

    // Value access
    constexpr T& value() & {
        if (!has_value_) {
            throw bad_optional_access();
        }
        return storage_.value;
    }

    constexpr const T& value() const& {
        if (!has_value_) {
            throw bad_optional_access();
        }
        return storage_.value;
    }

    constexpr T&& value() && {
        if (!has_value_) {
            throw bad_optional_access();
        }
        return std::move(storage_.value);
    }

    constexpr const T&& value() const&& {
        if (!has_value_) {
            throw bad_optional_access();
        }
        return std::move(storage_.value);
    }

    // Value or default
    template <typename U>
    constexpr T value_or(U&& default_value) const& {
        return has_value_ ? storage_.value : static_cast<T>(std::forward<U>(default_value));
    }

    template <typename U>
    constexpr T value_or(U&& default_value) && {
        return has_value_ ? std::move(storage_.value)
                          : static_cast<T>(std::forward<U>(default_value));
    }

    // Swap
    void swap(optional& other) noexcept(std::is_nothrow_swappable_v<T> &&
                                        std::is_nothrow_move_constructible_v<T>) {
        using std::swap;
        if (has_value_ && other.has_value_) {
            swap(storage_.value, other.storage_.value);
        } else if (has_value_) {
            new (&other.storage_.value) T(std::move(storage_.value));
            storage_.value.~T();
            other.has_value_ = true;
            has_value_ = false;
        } else if (other.has_value_) {
            new (&storage_.value) T(std::move(other.storage_.value));
            other.storage_.value.~T();
            has_value_ = true;
            other.has_value_ = false;
        }
    }

    // Reset to empty state
    void reset() noexcept {
        if (has_value_) {
            storage_.value.~T();
            has_value_ = false;
        }
    }

    // Emplace
    template <typename... Args>
    T& emplace(Args&&... args) noexcept(std::is_nothrow_constructible_v<T, Args...>) {
        reset();
        new (&storage_.value) T(std::forward<Args>(args)...);
        has_value_ = true;
        return storage_.value;
    }

private:
    optional_storage<T> storage_;
    bool has_value_;
};

// Comparison operators
template <typename T>
constexpr bool operator==(const optional<T>& lhs, const optional<T>& rhs) {
    if (lhs.has_value() != rhs.has_value()) return false;
    if (!lhs.has_value()) return true;
    return *lhs == *rhs;
}

template <typename T>
constexpr bool operator!=(const optional<T>& lhs, const optional<T>& rhs) {
    return !(lhs == rhs);
}

template <typename T>
constexpr bool operator<(const optional<T>& lhs, const optional<T>& rhs) {
    if (!rhs.has_value()) return false;
    if (!lhs.has_value()) return true;
    return *lhs < *rhs;
}

template <typename T>
constexpr bool operator<=(const optional<T>& lhs, const optional<T>& rhs) {
    if (!lhs.has_value()) return true;
    if (!rhs.has_value()) return false;
    return *lhs <= *rhs;
}

template <typename T>
constexpr bool operator>(const optional<T>& lhs, const optional<T>& rhs) {
    if (!lhs.has_value()) return false;
    if (!rhs.has_value()) return true;
    return *lhs > *rhs;
}

template <typename T>
constexpr bool operator>=(const optional<T>& lhs, const optional<T>& rhs) {
    if (!rhs.has_value()) return true;
    if (!lhs.has_value()) return false;
    return *lhs >= *rhs;
}

// Comparison with nullopt
template <typename T>
constexpr bool operator==(const optional<T>& opt, nullopt_t) noexcept {
    return !opt.has_value();
}

template <typename T>
constexpr bool operator==(nullopt_t, const optional<T>& opt) noexcept {
    return !opt.has_value();
}

template <typename T>
constexpr bool operator!=(const optional<T>& opt, nullopt_t) noexcept {
    return opt.has_value();
}

template <typename T>
constexpr bool operator!=(nullopt_t, const optional<T>& opt) noexcept {
    return opt.has_value();
}

// Comparison with value
template <typename T>
constexpr bool operator==(const optional<T>& opt, const T& value) {
    return opt.has_value() && *opt == value;
}

template <typename T>
constexpr bool operator==(const T& value, const optional<T>& opt) {
    return opt.has_value() && value == *opt;
}

template <typename T>
constexpr bool operator!=(const optional<T>& opt, const T& value) {
    return !opt.has_value() || *opt != value;
}

template <typename T>
constexpr bool operator!=(const T& value, const optional<T>& opt) {
    return !opt.has_value() || value != *opt;
}

// Swap function
template <typename T>
void swap(optional<T>& lhs, optional<T>& rhs) noexcept(noexcept(lhs.swap(rhs))) {
    lhs.swap(rhs);
}

#endif // OPTIONAL_HPP
