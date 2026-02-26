#pragma once

#include <cstring>
#include <iterator>
#include <stdexcept>
#include <utility>

class String {
  public:
    using iterator = char*;
    using const_iterator = const char*;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    iterator begin() noexcept { return data_; }
    iterator end() noexcept { return data_ + size_; }

    const_iterator begin() const noexcept { return data_; }
    const_iterator end() const noexcept { return data_ + size_; }

    const_iterator cbegin() const noexcept { return data_; }
    const_iterator cend() const noexcept { return data_ + size_; }

    reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }

    reverse_iterator rend() noexcept { return reverse_iterator(begin()); }

    const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(end()); }

    const_reverse_iterator rend() const noexcept { return const_reverse_iterator(begin()); }

    const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(cend()); }

    const_reverse_iterator crend() const noexcept { return const_reverse_iterator(cbegin()); }

    String() : capacity_(0), size_(0), data_(new char[1]{'\0'}) {}

    ~String() { delete[] data_; }

    String(size_t sz, char c) {
        data_ = new char[sz + 1];
        std::memset(data_, c, sz);
        data_[sz] = '\0';
        capacity_ = size_ = sz;
    }

    String(const char* data) {
        if (data) {
            const size_t len = std::strlen(data);
            data_ = new char[len + 1];
            std::memcpy(data_, data, len + 1);
            capacity_ = size_ = len;
        } else {
            data_ = new char[1]{'\0'};
            capacity_ = size_ = 0;
        }
    }

    String(const String& other) {
        data_ = new char[other.size_ + 1];
        std::memcpy(data_, other.data_, other.size_ + 1);
        capacity_ = size_ = other.size_;
    }

    String(String&& other) noexcept
        : capacity_(std::exchange(other.capacity_, 0)), size_(std::exchange(other.size_, 0)),
          data_(std::exchange(other.data_, new char[1]{'\0'})) {}

    String& operator=(const String& other) {
        if (this != &other) {
            String tmp(other);
            swap(tmp);
        }
        return *this;
    }

    String& operator=(String&& other) noexcept {
        if (this != &other) {
            delete[] data_;
            data_ = std::exchange(other.data_, new char[1]{'\0'});
            capacity_ = std::exchange(other.capacity_, 0);
            size_ = std::exchange(other.size_, 0);
        }
        return *this;
    }

    void push_back(char c) {
        if (capacity_ == size_) {
            size_t new_cap = capacity_ == 0 ? 1 : capacity_ * 2;
            reserve(new_cap);
        }
        data_[size_++] = c;
        data_[size_] = '\0';
    }

    void pop_back() {
        if (size_ == 0) { return; }
        data_[--size_] = '\0';
    }

    void reserve(size_t capacity) {
        if (capacity_ < capacity) {
            char* tmp = new char[capacity + 1];
            std::memcpy(tmp, data_, size_ + 1);
            delete[] data_;
            data_ = tmp;
            capacity_ = capacity;
        }
    }

    void swap(String& other) noexcept {
        std::swap(data_, other.data_);
        std::swap(capacity_, other.capacity_);
        std::swap(size_, other.size_);
    }

    char* data() noexcept { return data_; }

    const char* data() const noexcept { return data_; }

    const char* c_str() const noexcept { return data_; }

    char& at(size_t i) {
        if (i >= size_) { throw std::out_of_range("Index out of range"); }
        return data_[i];
    }

    const char& at(size_t i) const {
        if (i >= size_) { throw std::out_of_range("Index out of range"); }
        return data_[i];
    }

    char& operator[](size_t i) noexcept { return data_[i]; }

    const char& operator[](size_t i) const noexcept { return data_[i]; }

    size_t capacity() const noexcept { return capacity_; }

    size_t size() const noexcept { return size_; }

    bool empty() const noexcept { return size_ == 0; }

    friend bool operator==(const String& a, const String& b) {
        return a.size_ == b.size_ && std::memcmp(a.data_, b.data_, a.size_) == 0;
    }

    friend void swap(String& a, String& b) noexcept { a.swap(b); }

  private:
    size_t capacity_;
    size_t size_;
    char* data_;
};
