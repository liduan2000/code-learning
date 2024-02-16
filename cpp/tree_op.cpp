#include <algorithm>
#include <iostream>
#include <queue>
#include <stack>
#include <string>
#include <vector>

using namespace std;

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr){};
    TreeNode(int v) : val(v), left(nullptr), right(nullptr){};
    TreeNode(int v, TreeNode *l, TreeNode *r) : val(v), left(l), right(r){};
};

class BinaryTree {
   public:
    TreeNode *createTree() {
        string s;
        cin >> s;
        if (s == "#") return nullptr;
        TreeNode *node = new TreeNode(stoi(s));
        node->left = createTree();
        node->right = createTree();
        return node;
    }

    void recursivePreorderTraversal(TreeNode *root) {
        if (!root) return;
        cout << root->val << " ";
        recursivePreorderTraversal(root->left);
        recursivePreorderTraversal(root->right);
    }

    void recursiveInorderTraversal(TreeNode *root) {
        if (!root) return;
        recursiveInorderTraversal(root->left);
        cout << root->val << " ";
        recursiveInorderTraversal(root->right);
    }

    void recursivePostorderTraversal(TreeNode *root) {
        if (!root) return;
        recursivePostorderTraversal(root->left);
        recursivePostorderTraversal(root->right);
        cout << root->val << " ";
    }

    void iterativePreorderTraversal(TreeNode *root) {
        if (!root) return;
        stack<TreeNode *> st;
        st.push(root);
        while (!st.empty()) {
            TreeNode *cur = st.top();
            st.pop();
            cout << cur->val << " ";
            if (cur->right) st.push(cur->right);
            if (cur->left) st.push(cur->left);
        }
        cout << endl;
    }

    void iterativeInorderTraversal(TreeNode *root) {
        if (!root) return;
        stack<TreeNode *> st;
        TreeNode *cur = root;
        while (cur || !st.empty()) {
            if (cur) {
                st.push(cur);
                cur = cur->left;
            } else {
                cur = st.top();
                st.pop();
                cout << cur->val << " ";
                cur = cur->right;
            }
        }
        cout << endl;
    }

    void iterativePostorderTraversal(TreeNode *root) {
        if (!root) return;
        stack<TreeNode *> st;
        st.push(root);
        vector<int> nums;
        while (!st.empty()) {
            TreeNode *cur = st.top();
            st.pop();
            nums.push_back(cur->val);
            if (cur->left) st.push(cur->left);
            if (cur->right) st.push(cur->right);
        }
        reverse(nums.begin(), nums.end());
        for (int num : nums) {
            cout << num << " ";
        }
        cout << endl;
    }

    void levelorderTraversal(TreeNode *root) {
        if (!root) return;
        queue<TreeNode *> que;
        que.push(root);
        while (!que.empty()) {
            int size = que.size();
            while (size--) {
                TreeNode *cur = que.front();
                que.pop();
                cout << cur->val << " ";
                if (cur->left) que.push(cur->left);
                if (cur->right) que.push(cur->right);
            }
        }
        cout << endl;
    }

    void deleteTree(TreeNode *root) {
        if (!root) return;
        deleteTree(root->left);
        deleteTree(root->right);
        delete root;
    }
};

int main() {
    BinaryTree tree;
    TreeNode *root = tree.createTree();
    tree.iterativePreorderTraversal(root);
    tree.iterativeInorderTraversal(root);
    tree.iterativePostorderTraversal(root);
    tree.levelorderTraversal(root);
    tree.deleteTree(root);
}