#pragma once
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <deque>
#include <iostream>
#include <limits>
#include <list>
#include <map>
#include <numeric>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

struct ListNode {
    int val;
    ListNode* next;
    ListNode() : val(0), next(nullptr) {};
    ListNode(int v) : val(v), next(nullptr) {};
    ListNode(int v, ListNode* n) : val(v), next(n) {};
};

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {};
    TreeNode(int v) : val(v), left(nullptr), right(nullptr) {};
    TreeNode(int v, TreeNode* l, TreeNode* r) : val(v), left(l), right(r) {};
};

struct DLLNode {
    int key;
    int value;
    DLLNode* prev;
    DLLNode* next;
    DLLNode() : key(0), value(0), prev(nullptr), next(nullptr) {}
    DLLNode(int k, int v) : key(k), value(v), prev(nullptr), next(nullptr) {}
};