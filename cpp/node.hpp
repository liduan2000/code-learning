#pragma once

struct ListNode {
    int val;
    ListNode* next;
    ListNode() : val(0), next(nullptr) {};
    ListNode(int v) : val(v), next(nullptr) {};
    ListNode(int _val, ListNode* _next) : val(_val), next(_next) {};
};

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {};
    TreeNode(int _val) : val(_val), left(nullptr), right(nullptr) {};
    TreeNode(int _val, TreeNode* _left, TreeNode* _right) : val(_val), left(_left), right(_right) {};
};

struct DLLNode {
    int key;
    int value;
    DLLNode* prev;
    DLLNode* next;
    DLLNode() : key(0), value(0), prev(nullptr), next(nullptr) {}
    DLLNode(int _key, int _value) : key(_key), value(_value), prev(nullptr), next(nullptr) {}
};
