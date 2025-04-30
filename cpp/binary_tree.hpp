#pragma once

#include <algorithm>
#include <iostream>
#include <queue>
#include <stack>
#include <string>
#include <vector>

#include "node.hpp"

class BinaryTree {
  public:
    TreeNode* createTree() {
        std::string s;
        std::cin >> s;
        if (s == "#") return nullptr;
        TreeNode* node = new TreeNode(stoi(s));
        node->left = createTree();
        node->right = createTree();
        return node;
    }

    void recursivePreorderTraversal(TreeNode* root) {
        if (!root) return;
        std::cout << root->val << " ";
        recursivePreorderTraversal(root->left);
        recursivePreorderTraversal(root->right);
    }

    void recursiveInorderTraversal(TreeNode* root) {
        if (!root) return;
        recursiveInorderTraversal(root->left);
        std::cout << root->val << " ";
        recursiveInorderTraversal(root->right);
    }

    void recursivePostorderTraversal(TreeNode* root) {
        if (!root) return;
        recursivePostorderTraversal(root->left);
        recursivePostorderTraversal(root->right);
        std::cout << root->val << " ";
    }

    void iterativePreorderTraversal(TreeNode* root) {
        if (!root) return;
        std::stack<TreeNode*> st;
        st.push(root);
        while (!st.empty()) {
            TreeNode* cur = st.top();
            st.pop();
            std::cout << cur->val << " ";
            if (cur->right) st.push(cur->right);
            if (cur->left) st.push(cur->left);
        }
        std::cout << std::endl;
    }

    void iterativeInorderTraversal(TreeNode* root) {
        if (!root) return;
        std::stack<TreeNode*> st;
        TreeNode* cur = root;
        while (cur || !st.empty()) {
            if (cur) {
                st.push(cur);
                cur = cur->left;
            } else {
                cur = st.top();
                st.pop();
                std::cout << cur->val << " ";
                cur = cur->right;
            }
        }
        std::cout << std::endl;
    }

    void iterativePostorderTraversal(TreeNode* root) {
        if (!root) return;
        std::stack<TreeNode*> st;
        st.push(root);
        std::vector<int> nums;
        while (!st.empty()) {
            TreeNode* cur = st.top();
            st.pop();
            nums.push_back(cur->val);
            if (cur->left) st.push(cur->left);
            if (cur->right) st.push(cur->right);
        }
        reverse(nums.begin(), nums.end());
        for (int num : nums) { std::cout << num << " "; }
        std::cout << std::endl;
    }

    void levelorderTraversal(TreeNode* root) {
        if (!root) return;
        std::queue<TreeNode*> q;
        q.push(root);
        while (!q.empty()) {
            int size = q.size();
            while (size--) {
                TreeNode* cur = q.front();
                q.pop();
                std::cout << cur->val << " ";
                if (cur->left) q.push(cur->left);
                if (cur->right) q.push(cur->right);
            }
        }
        std::cout << std::endl;
    }

    void deleteTree(TreeNode* root) {
        if (!root) return;
        deleteTree(root->left);
        deleteTree(root->right);
        delete root;
    }
};
