#pragma once

#include <functional>
#include <stdexcept>
#include <vector>

template <typename T, typename Compare = std::less<T>>
class PriorityQueue {
  public:
    void push(T data) {
        heap_.push_back(data);
        shiftUp(heap_.size() - 1);
    }

    void pop() {
        if (empty()) { throw std::runtime_error("PriorityQueue is empty"); }
        heap_[0] = heap_.back();
        heap_.pop_back();
        shiftDown(0);
    }

    T top() {
        if (empty()) { throw std::runtime_error("PriorityQueue is empty"); }
        return heap_[0];
    }

    bool empty() { return heap_.empty(); }

    size_t size() { return heap_.size(); }

  private:
    std::vector<T> heap_;
    Compare comp_;

    void shiftUp(int idx) {
        while (idx > 0) {
            int parent = (idx - 1) / 2;
            if (comp_(heap_[idx], heap_[parent])) { break; }
            std::swap(heap_[idx], heap_[parent]);
            idx = parent;
        }
    }

    void shiftDown(int idx) {
        int n = heap_.size();
        while (idx * 2 + 1 < n) {
            int leftChild = idx * 2 + 1;
            int rightChild = idx * 2 + 2;
            int smallest = idx;
            if (leftChild < n && !comp_(heap_[leftChild], heap_[smallest])) { smallest = leftChild; }
            if (rightChild < n && !comp_(heap_[rightChild], heap_[smallest])) { smallest = rightChild; }
            if (smallest == idx) { break; }
            std::swap(heap_[idx], heap_[smallest]);
            idx = smallest;
        }
    }
};
