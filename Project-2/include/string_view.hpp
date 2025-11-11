#ifndef PROJECT2_STRING_VIEW_HPP
#define PROJECT2_STRING_VIEW_HPP

// Minimal C++17-compatible implementation of a string_view-like class for educational use.
// Placed into namespace project2 to avoid colliding with the standard library's std::string_view.

#include <cstddef>
#include <cstring>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <iterator>
#include <ostream>

namespace project2 {

class string_view {
public:
    using value_type = char;
    using pointer = const value_type*;
    using const_pointer = const value_type*;
    using reference = const value_type&;
    using const_reference = const value_type&;
    using const_iterator = const_pointer;
    using iterator = const_iterator;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

    // constructors
    constexpr string_view() noexcept : data_(nullptr), size_(0) {}
    constexpr string_view(const char* s, size_type count) : data_(s), size_(count) {}
    constexpr string_view(const char* s) : data_(s), size_(s ? std::strlen(s) : 0) {}
    string_view(const std::string& s) noexcept : data_(s.data()), size_(s.size()) {}

    // observers
    constexpr const_pointer data() const noexcept { return data_; }
    constexpr size_type size() const noexcept { return size_; }
    constexpr size_type length() const noexcept { return size_; }
    constexpr bool empty() const noexcept { return size_ == 0; }

    constexpr const_reference operator[](size_type pos) const { return data_[pos]; }
    constexpr const_reference at(size_type pos) const {
        if (pos >= size_) throw std::out_of_range("string_view::at");
        return data_[pos];
    }
    constexpr const_reference front() const { return data_[0]; }
    constexpr const_reference back() const { return data_[size_-1]; }

    // modifiers
    constexpr void remove_prefix(size_type n) noexcept {
        if (n > size_) n = size_;
        data_ += n;
        size_ -= n;
    }
    constexpr void remove_suffix(size_type n) noexcept {
        if (n > size_) size_ = 0;
        else size_ -= n;
    }

    void swap(string_view& other) noexcept {
        std::swap(data_, other.data_);
        std::swap(size_, other.size_);
    }

    // iterators
    constexpr const_iterator begin() const noexcept { return data_; }
    constexpr const_iterator end() const noexcept { return data_ + size_; }
    constexpr const_iterator cbegin() const noexcept { return begin(); }
    constexpr const_iterator cend() const noexcept { return end(); }

    // conversion
    std::string to_string() const { return std::string(data_, size_); }

    // substr
    constexpr string_view substr(size_type pos = 0, size_type count = npos) const {
        if (pos > size_) throw std::out_of_range("string_view::substr");
        size_type rcount = std::min(count, size_ - pos);
        return string_view(data_ + pos, rcount);
    }

    // compare: mimic std::string_view::compare behavior
    int compare(string_view other) const noexcept {
        const size_type r = std::min(size_, other.size_);
        int cmp = std::memcmp(data_, other.data_, r);
        if (cmp != 0) return cmp;
        if (size_ < other.size_) return -1;
        if (size_ > other.size_) return 1;
        return 0;
    }

    // find (naive implementations)
    static constexpr size_type npos = static_cast<size_type>(-1);

    size_type find(char c, size_type pos = 0) const noexcept {
        if (pos >= size_) return npos;
        const char* p = static_cast<const char*>(std::memchr(data_ + pos, c, size_ - pos));
        if (!p) return npos;
        return static_cast<size_type>(p - data_);
    }

    size_type find(const char* s, size_type pos, size_type count) const noexcept {
        if (!s) return npos;
        if (count == 0) return (pos <= size_) ? pos : npos;
        if (pos > size_ || count > size_ - pos) return npos;
        // naive search
        for (size_type i = pos; i + count <= size_; ++i) {
            if (std::memcmp(data_ + i, s, count) == 0) return i;
        }
        return npos;
    }

    size_type find(string_view sv, size_type pos = 0) const noexcept {
        return find(sv.data_, pos, sv.size_);
    }

    size_type find(const char* s, size_type pos = 0) const noexcept {
        if (!s) return npos;
        return find(s, pos, std::strlen(s));
    }

    size_type rfind(char c, size_type pos = npos) const noexcept {
        if (size_ == 0) return npos;
        size_type i = (pos >= size_) ? size_ - 1 : pos;
        for (;; --i) {
            if (data_[i] == c) return i;
            if (i == 0) break;
        }
        return npos;
    }

    size_type rfind(string_view sv, size_type pos = npos) const noexcept {
        if (sv.size_ > size_) return npos;
        size_type limit = (pos >= size_) ? size_ - sv.size_ : std::min(pos, size_ - sv.size_);
        for (size_type i = limit + 1; i > 0; --i) {
            size_type j = i - 1;
            if (std::memcmp(data_ + j, sv.data_, sv.size_) == 0) return j;
        }
        return npos;
    }

    // starts_with / ends_with helpers
    bool starts_with(string_view sv) const noexcept {
        if (sv.size_ > size_) return false;
        return std::memcmp(data_, sv.data_, sv.size_) == 0;
    }
    bool ends_with(string_view sv) const noexcept {
        if (sv.size_ > size_) return false;
        return std::memcmp(data_ + (size_ - sv.size_), sv.data_, sv.size_) == 0;
    }

private:
    const char* data_;
    size_type size_;
};

// relational operators
inline bool operator==(const string_view& a, const string_view& b) noexcept { return a.size() == b.size() && (a.size() == 0 || std::memcmp(a.data(), b.data(), a.size()) == 0); }
inline bool operator!=(const string_view& a, const string_view& b) noexcept { return !(a == b); }
inline bool operator<(const string_view& a, const string_view& b) noexcept { return a.compare(b) < 0; }
inline bool operator<=(const string_view& a, const string_view& b) noexcept { return a.compare(b) <= 0; }
inline bool operator>(const string_view& a, const string_view& b) noexcept { return a.compare(b) > 0; }
inline bool operator>=(const string_view& a, const string_view& b) noexcept { return a.compare(b) >= 0; }

// stream insertion helper
inline std::ostream& operator<<(std::ostream& os, const string_view& sv) {
    if (!sv.empty()) os.write(sv.data(), static_cast<std::streamsize>(sv.size()));
    return os;
}

} // namespace project2

#endif // PROJECT2_STRING_VIEW_HPP
