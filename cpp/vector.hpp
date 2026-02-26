#pragma once

#include <algorithm>
#include <memory>
#include <stdexcept>

template <typename T>
class Vector {
  public:
    using iterator = T*;
    using const_iterator = const T*;
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

    iterator data() noexcept { return data_; }
    const_iterator data() const noexcept { return data_; }

    Vector() : capacity_(0), size_(0), data_(nullptr) {}

    explicit Vector(size_t sz) {
        data_ = static_cast<T*>(::operator new(sizeof(T) * sz));
        try {
            std::uninitialized_value_construct_n(data_, sz);
        } catch (...) {
            ::operator delete(data_);
            throw;
        }
        capacity_ = size_ = sz;
    }

    Vector(size_t sz, const T& val) {
        data_ = static_cast<T*>(::operator new(sizeof(T) * sz));
        try {
            std::uninitialized_fill_n(data_, sz, val);
        } catch (...) {
            ::operator delete(data_);
            throw;
        }
        capacity_ = size_ = sz;
    }

    Vector(std::initializer_list<T> lst) {
        data_ = static_cast<T*>(::operator new(sizeof(T) * lst.size()));
        try {
            std::uninitialized_copy_n(lst.begin(), lst.size(), data_);
        } catch (...) {
            ::operator delete(data_);
            throw;
        }
        capacity_ = size_ = lst.size();
    }

    ~Vector() {
        std::destroy_n(data_, size_);
        ::operator delete(data_);
    }

    Vector(const Vector& other) {
        data_ = static_cast<T*>(::operator new(sizeof(T) * other.size_));
        try {
            std::uninitialized_copy_n(other.data_, other.size_, data_);
        } catch (...) {
            ::operator delete(data_);
            throw;
        }
        capacity_ = size_ = other.size_;
    }

    Vector(Vector&& other) noexcept
        : capacity_(std::exchange(other.capacity_, 0)), size_(std::exchange(other.size_, 0)),
          data_(std::exchange(other.data_, nullptr)) {}

    Vector& operator=(const Vector& other) {
        if (this != &other) {
            Vector tmp(other);
            swap(tmp);
        }
        return *this;
    }

    Vector& operator=(Vector&& other) noexcept {
        if (this != &other) {
            std::destroy_n(data_, size_);
            ::operator delete(data_);
            capacity_ = std::exchange(other.capacity_, 0);
            size_ = std::exchange(other.size_, 0);
            data_ = std::exchange(other.data_, nullptr);
        }
        return *this;
    }

    void push_back(const T& val) { emplace_back(val); }

    template <typename... Args>
    void emplace_back(Args&&... args) {
        if (capacity_ == size_) {
            size_t new_cap = capacity_ == 0 ? 1 : capacity_ * 2;
            reserve(new_cap);
        }
        std::construct_at(data_ + size_, std::forward<Args>(args)...);
        ++size_;
    }

    void pop_back() {
        if (size_ > 0) {
            --size_;
            std::destroy_at(data_ + size_);
        }
    }

    void reserve(size_t new_cap) {
        if (new_cap <= capacity_) { return; }
        T* tmp = static_cast<T*>(::operator new(sizeof(T) * new_cap));
        try {
            if constexpr (std::is_nothrow_move_constructible_v<T> || !std::is_copy_constructible_v<T>) {
                std::uninitialized_move_n(data_, size_, tmp);
            } else {
                std::uninitialized_copy_n(data_, size_, tmp);
            }
        } catch (...) {
            ::operator delete(tmp);
            throw;
        }
        std::destroy_n(data_, size_);
        ::operator delete(data_);
        data_ = tmp;
        capacity_ = new_cap;
    }

    void clear() noexcept {
        std::destroy_n(data_, size_);
        size_ = 0;
    }

    void swap(Vector& other) noexcept {
        std::swap(capacity_, other.capacity_);
        std::swap(size_, other.size_);
        std::swap(data_, other.data_);
    }

    T& at(size_t i) {
        if (i >= size_) { throw std::out_of_range("Index out of range"); }
        return data_[i];
    }

    const T& at(size_t i) const {
        if (i >= size_) { throw std::out_of_range("Index out of range"); }
        return data_[i];
    }

    T& operator[](size_t i) noexcept { return data_[i]; }

    const T& operator[](size_t i) const noexcept { return data_[i]; }

    size_t capacity() const noexcept { return capacity_; }

    size_t size() const noexcept { return size_; }

    friend void swap(Vector& a, Vector& b) noexcept { a.swap(b); }

  private:
    size_t capacity_;
    size_t size_;
    T* data_;
};
