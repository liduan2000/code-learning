#pragma once

#include <functional>
#include <stdexcept>
#include <utility>
#include <vector>

template <typename T, typename Comp = std::less<T>>
class PriorityQueue {
  public:
    void push(const T& val) {
        data_.push_back(val);
        shiftUp(data_.size() - 1);
    }

    template <typename... Args>
    void emplace(Args&&... args) {
        data_.emplace_back(std::forward<Args>(args)...);
        shiftUp(data_.size() - 1);
    }

    void pop() {
        if (empty()) { throw std::runtime_error("PriorityQueue is empty"); }
        data_[0] = std::move(data_.back());
        data_.pop_back();
        if (!empty()) { shiftDown(0); }
    }

    const T& top() const {
        if (empty()) { throw std::runtime_error("PriorityQueue is empty"); }
        return data_.front();
    }

    bool empty() const { return data_.empty(); }

    size_t size() const { return data_.size(); }

  private:
    void shiftUp(size_t idx) {
        while (idx > 0) {
            size_t parent = (idx - 1) >> 1;
            if (comp_(data_[idx], data_[parent])) { break; }
            std::swap(data_[parent], data_[idx]);
            idx = parent;
        }
    }

    void shiftDown(size_t idx) {
        const size_t n = size();
        while (idx * 2 + 1 < n) {
            size_t lChild = idx * 2 + 1, rChild = idx * 2 + 2;
            size_t best = idx;
            if (lChild < n && comp_(data_[best], data_[lChild])) { best = lChild; }
            if (rChild < n && comp_(data_[best], data_[rChild])) { best = rChild; }
            if (best == idx) { break; }
            std::swap(data_[idx], data_[best]);
            idx = best;
        }
    }

    std::vector<T> data_;
    Comp comp_;
};
