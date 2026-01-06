#pragma once

#include <vector>

class SegmentTree {
  public:
    SegmentTree(const std::vector<int>& nums) : n_(nums.size()), tree_(4 * n_), m_(4 * n_) {
        static auto build = [&](auto&& build, int s, int t, int p) {
            if (s == t) {
                tree_[p] = nums[s];
                return;
            }
            int m = s + ((t - s) >> 1);
            build(build, s, m, 2 * p);
            build(build, m + 1, t, 2 * p + 1);
            tree_[p] = tree_[2 * p] + tree_[2 * p + 1];
        };
        build(build, 0, n_ - 1, 1);
    }

    int getSum(int l, int r) { return getSum(l, r, 0, n_, 1); }

    void addRange(int l, int r, int x) { addRange(l, r, x, 0, n_, 1); }

  private:
    int getSum(int l, int r, int s, int t, int p) {
        if (l <= s && t <= r) { return tree_[p]; }

        int m = s + ((t - s) >> 1);
        if (m_[p]) {
            tree_[2 * p] += (m - s + 1) * m_[p];
            tree_[2 * p + 1] += (t - m) * m_[p];
            m_[2 * p] += m_[p];
            m_[2 * p + 1] += m_[p];
            m_[p] = 0;
        }
        int sum = 0;
        if (l <= m) { sum += getSum(l, r, s, m, 2 * p); }
        if (r > m) { sum += getSum(l, r, m + 1, t, 2 * p + 1); }
        return sum;
    }

    void addRange(int l, int r, int x, int s, int t, int p) {
        if (l <= s && t <= r) {
            tree_[p] += (t - s + 1) * x;
            m_[p] += x;
            return;
        }
        int m = s + ((t - s) >> 1);
        if (m_[p] && s != t) {
            tree_[2 * p] += (m - s + 1) * x;
            tree_[2 * p + 1] += (t - m) * x;
            m_[2 * p] += x;
            m_[2 * p + 1] += x;
            m_[p] = 0;
        }
        if (l <= m) { addRange(l, r, x, s, m, 2 * p); }
        if (r > m) { addRange(l, r, x, m + 1, t, 2 * p + 1); }
        tree_[p] = tree_[2 * p] + tree_[2 * p + 1];
    }

    int n_;
    std::vector<int> tree_;
    std::vector<int> m_;
};