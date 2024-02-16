#include "node.h"

ListNode *CreateList() {
    string s_val;
    cin >> s_val;
    if (s_val == "#") return nullptr;
    ListNode *list_node = new ListNode(stoi(s_val));
    list_node->next = CreateList();
    return list_node;
}

TreeNode *CreateTree() {
    string s_val;
    cin >> s_val;
    if (s_val == "#") return nullptr;
    TreeNode *tree_node = new TreeNode(stoi(s_val));
    tree_node->left = CreateTree();
    tree_node->right = CreateTree();
    return tree_node;
}