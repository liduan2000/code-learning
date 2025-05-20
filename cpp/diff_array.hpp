#pragma once

#include <iostream>
#include <vector>

class DiffArray {
  public:
    DiffArray(std::vector<int>& arr) : arr_(arr), diff_(arr.size() + 1, 0) {
        diff_[0] = arr_[0];
        for (int i = 1; i < arr_.size(); ++i) { diff_[i] = arr_[i] - arr_[i - 1]; }
    }

    void update(int l, int r, int x) {
        if (l < 0 || r >= static_cast<int>(arr_.size()) || l > r) { return; }
        diff_[l] += x;
        diff_[r + 1] -= x;
    }

    void restore() {
        for (int i = 0; i < arr_.size(); ++i) {
            if (i == 0) {
                arr_[i] = diff_[i];
            } else {
                arr_[i] = diff_[i] + arr_[i - 1];
            }
        }
    }

  private:
    std::vector<int>& arr_;

    std::vector<int> diff_;
};
